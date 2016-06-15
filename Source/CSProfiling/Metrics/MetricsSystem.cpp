//
//  MetricsSystem.cpp
//  CSProfiling
//  Created by Angela Gross on 31/03/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <CSProfiling/Metrics/MetricsSystem.h>
#include <CSProfiling/Shiny/Shiny.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace CSProfiling
{
	namespace
	{
		const std::string k_runNumVarName = "runNum";
		const std::string k_dateVarName = "date";
		const std::string k_runTimeVarName = "runTime";
		const std::string k_particleTimingHeader = "particles_emitted,run_num,left_side_hit,right_side_hit,render_ball_called";
		const std::string k_baseFilePath = "Metrics/particleTiming_[var=date]_[var=runTime]-seconds_[var=runNum]-run.csv";
		const std::string k_baseShinyFilePath = "Metrics/shinyProfile_[var=date]_[var=runNum].txt";
		const CSCore::StorageLocation k_storageLocation = CSCore::StorageLocation::k_saveData;
	}

	CS_DEFINE_NAMEDTYPE(MetricsSystem);

	//---------------------------------------------------
	//---------------------------------------------------
	MetricsSystemUPtr MetricsSystem::Create(u32 in_maxRunNum, u32 in_runTime, u32 in_minParticles, u32 in_maxParticles, u32 in_particlesStep)
	{
		return MetricsSystemUPtr(new MetricsSystem(in_maxRunNum, in_runTime, in_minParticles, in_maxParticles, in_particlesStep));
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void MetricsSystem::OnDestroy()
	{
		m_gameTimer.Stop();
		m_timerStoppedEvent.CloseAllConnections();
		m_gameTimer.CloseConnection(m_timerStoppedConnection.get());
		m_timerStoppedConnection->Close();
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	bool MetricsSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
	{
		return in_interfaceId == MetricsSystem::InterfaceID;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	MetricsSystem::MetricsSystem(u32 in_maxRunNum, u32 in_runTime, u32 in_minParticles, u32 in_maxParticles, u32 in_particlesStep)
		: k_maxRunNum(in_maxRunNum), k_runTime(in_runTime), k_minParticles(in_minParticles), k_maxParticles(in_maxParticles), k_particlesStep(in_particlesStep)
	{
        m_currentParticles = k_minParticles;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	CSCore::IConnectableEvent<MetricsSystem::TimerStoppedDelegate>& MetricsSystem::GetTimerStoppedEvent()
	{
		return m_timerStoppedEvent;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void MetricsSystem::UpdateSidesHit(u32 in_leftSideHit, u32 in_rightSideHit)
	{
		m_leftSideHit = in_leftSideHit;
		m_rightSideHit = in_rightSideHit;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void MetricsSystem::IncrementRenderBallCalled()
	{
		m_renderBallCalled++;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void MetricsSystem::ResetMetricVariables()
	{
		m_leftSideHit = 0;
		m_rightSideHit = 0;
		m_renderBallCalled = 0;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	bool MetricsSystem::AreRunsOver() const
	{
		return m_runNum >= k_maxRunNum;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	bool MetricsSystem::AreAllRunsOver() const
	{
		return (m_currentParticles >= k_maxParticles) && AreRunsOver();
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	u32 MetricsSystem::IncrementParticles()
	{
		// reset run num
		m_runNum = 0;
        
        // increment the particles per emission by the step
        m_currentParticles += k_particlesStep;
		
        return m_currentParticles;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void MetricsSystem::StartTimer()
	{
		if (!m_gameTimer.IsTimerActive())
		{
			m_timerStoppedConnection = m_gameTimer.OpenConnection(k_runTime, CSCore::MakeDelegate(this, &MetricsSystem::OnTimerStopped));
			m_gameTimer.Start();
		}
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void MetricsSystem::OnTimerStopped()
	{
		BuildMetrics();
		ResetMetricVariables();
		m_runNum++;

		if (AreAllRunsOver())
		{
			OutputMetrics();
		}

		m_timerStoppedEvent.NotifyConnections();
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	void MetricsSystem::BuildMetrics()
	{
		// first put in the header if the output hasn't been initialized yet
		if (!m_isOutputInitialized)
		{
			m_metricsOSS << k_particleTimingHeader << "\r\n";
			m_isOutputInitialized = true;
		}

		m_metricsOSS << m_currentParticles << ","
			<< m_runNum << "," 
			<< m_leftSideHit << "," 
			<< m_rightSideHit << "," 
			<< m_renderBallCalled << "\r\n";
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	bool MetricsSystem::OutputMetrics()
	{
		bool isWritten = false;

		// create directory for metrics (won't create it if it already exists)
		bool isCreated = CSCore::Application::Get()->GetFileSystem()->CreateDirectoryPath(k_storageLocation, "Metrics");

		if (isCreated)
		{
			// get built metrics and clear stream
			std::string metricsStr = m_metricsOSS.str();
			m_metricsOSS.str("");
			m_metricsOSS.clear();

			// get current time as string
			std::ostringstream timeOSS;
			const time_t tmb = std::time(nullptr);
			auto tm = *localtime(&tmb);
			timeOSS << PUT_TIME(&tm, "%d-%m-%Y_%H-%M-%S");
			std::string timeStr = timeOSS.str();

			// build path
			std::string metricsPath = CSCore::StringUtils::InsertVariables
			(
				k_baseFilePath, 
				{ 
					std::make_pair(k_dateVarName, timeStr),
					std::make_pair(k_runTimeVarName, TO_STRING(k_runTime)),
					std::make_pair(k_runNumVarName, TO_STRING(k_maxRunNum))
				}
			);

			// write metrics
			isWritten = CSCore::Application::Get()->GetFileSystem()->WriteFile(k_storageLocation, metricsPath, metricsStr);
			
			if (!isWritten)
			{
				CS_LOG_ERROR("METRICS OUTPUT: Failed to write metrics file");
			}
			else
			{
				CS_LOG_VERBOSE("METRICS OUTPUT: Saved metrics file");
			}
		}
		else
		{
			CS_LOG_ERROR("METRICS OUTPUT: Failed to create directory path for metrics file");
		}

		// reset stream
		m_isOutputInitialized = false;

		return isWritten;
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	bool MetricsSystem::OutputShinyMetrics()
	{
		bool isWritten = false;

		// update all profiles
		PROFILE_UPDATE();

		// create directory for profiling (won't create it if it already exists)
		bool isCreated = CSCore::Application::Get()->GetFileSystem()->CreateDirectoryPath(k_storageLocation, "Metrics");

		if (isCreated)
		{
			// get both parts of profile string
			std::string profileStr = PROFILE_GET_FLAT_STRING();
			profileStr.append("\r\n\r\n");
			profileStr.append(PROFILE_GET_TREE_STRING());

			std::string profilePath = CSCore::StringUtils::InsertVariables(k_baseShinyFilePath, { std::make_pair(k_runNumVarName, TO_STRING(m_runNum)) });

			// write profile 
			isWritten = CSCore::Application::Get()->GetFileSystem()->WriteFile(k_storageLocation, profilePath, profileStr);

			if (!isWritten)
			{
				CS_LOG_ERROR("PROFILING OUTPUT: Failed to write shiny profile file");
			}
			else
			{
				CS_LOG_VERBOSE("PROFILING OUTPUT: Saved shiny profile file");
			}
		}
		else
		{
			CS_LOG_ERROR("PROFILING OUTPUT: Failed to create directory path for shiny profile file");
		}

		return isWritten;
	}


}