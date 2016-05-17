//
//  ParticleEffectComponentFactory.h
//  CSPong
//  Created by Angela Gross on 12/01/2016.
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

#ifndef _CSPROFILING_METRICS_METRICSSYSTEM_H_
#define _CSPROFILING_METRICS_METRICSSYSTEM_H_

#include <CSProfiling/ForwardDeclarations.h>

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/Event.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/Time/Timer.h>

namespace CSProfiling
{
	//-----------------------------------------------------------------------------------
	/// An App system that helps automate profiling parts of the system by keeping track
	/// of metrics (particle render method calls, ball render method calls, score) over
	/// a set period of time. When that time is up, the state should restart until the
	/// the number of runs are completed and all files are saved.
	///
	/// @author Angela Gross
	//-----------------------------------------------------------------------------------
	class MetricsSystem final : public CSCore::AppSystem
	{

	public:

		CS_DECLARE_NAMEDTYPE(MetricsSystem);

		using TimerStoppedDelegate = std::function<void()>;

		//------------------------------------------------------------------------------
		/// Creates a new instance of the system.
		///
		/// @author Angela Gross
		///
		/// @return The new instance.
		//------------------------------------------------------------------------------
		static MetricsSystemUPtr Create(u32 in_maxRunNum, u32 in_runTime, u32 in_minParticles, u32 in_maxParticles, u32 in_particlesStep);
		//------------------------------------------------------------------------------
		/// Destroys the object
		///
		/// @author Angela Gross
		//------------------------------------------------------------------------------
		void OnDestroy() override;
		//------------------------------------------------------------------------------
		/// @author Angela Gross
		///
		/// @param Comparison Type
		///
		/// @return Whether the class matches the comparison type
		//------------------------------------------------------------------------------
		bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
		//------------------------------------------------------------------------------
		/// @author Angela Gross
		///
		/// @return The timer stopped event
		//------------------------------------------------------------------------------
		CSCore::IConnectableEvent<TimerStoppedDelegate>& GetTimerStoppedEvent();
		//------------------------------------------------------------------------------
		/// Updates the "score", or the number of times the ball hits each side
		///
		/// @author Angela Gross
		///
		/// @param in_leftSideHit
		/// @param in_rightSideHit
		//------------------------------------------------------------------------------
		void UpdateSidesHit(u32 in_leftSideHit, u32 in_rightSideHit);
		//------------------------------------------------------------------------------
		/// Increment the number of times the render method for the ball is called
		///
		/// @author Angela Gross
		//------------------------------------------------------------------------------
		void IncrementRenderBallCalled();
		//------------------------------------------------------------------------------
		/// Resets sides hit, render ball and particles called variables
		///
		/// @author Angela Gross
		//------------------------------------------------------------------------------
		void ResetMetricVariables();
		//------------------------------------------------------------------------------
		/// @author Angela Gross
		///
		/// @return Whether or not all of the runs have executed.
		//------------------------------------------------------------------------------
		bool AreRunsOver() const;
		//------------------------------------------------------------------------------
		/// @author Angela Gross
		///
		/// @return Whether or not all of the runs for all particles have executed.
		//------------------------------------------------------------------------------
		bool AreAllRunsOver() const;
		//------------------------------------------------------------------------------
		/// Increments the current particles per emission by the step (given at
        /// construction)
		///
		/// @author Angela Gross
		///
		/// @return The current number of particles per emission
		//------------------------------------------------------------------------------
		u32 IncrementParticles();
		//------------------------------------------------------------------------------
		/// Starts the timer   
		///
		/// @author Angela Gross
		//------------------------------------------------------------------------------
		void StartTimer();
		//------------------------------------------------------------------------------
		/// Builds the metrics file with recorded metrics
		///
		/// @author Angela Gross
		//------------------------------------------------------------------------------
		void BuildMetrics();
		//------------------------------------------------------------------------------
		/// Writes the gathered metrics to the defined storage location (defined at 
		/// class creation) and file name. The file name will differ depending on what
		/// run 
		///
		/// @author Angela Gross
		///
		/// @return Whether or not the file was successfully written.
		//------------------------------------------------------------------------------
		bool OutputMetrics();
		//------------------------------------------------------------------------------
		/// Writes the shiny profile statistics to the defined storage location (defined
		/// at class creation) and file name. The file name will differ depending on 
		/// what run
		///
		/// @author Angela Gross
		///
		/// @return Whether or not the file was successfully written.
		//------------------------------------------------------------------------------
		bool OutputShinyMetrics();

	private:
		//------------------------------------------------------------------------------
		/// Private constructor to enforce use of factory method.
		///
		/// @author Angela Gross
		///
		/// @param in_maxRunNum
		/// @param in_runTime
		//------------------------------------------------------------------------------
		MetricsSystem(u32 in_maxRunNum, u32 in_runTime, u32 in_minParticles, u32 in_maxParticles, u32 in_particlesStep);
		//------------------------------------------------------------------------------
		/// The method to be called when the timer has "gone off" 
		///
		/// @author Angela Gross
		//------------------------------------------------------------------------------
		void OnTimerStopped();

	private:
		CSCore::Event<TimerStoppedDelegate> m_timerStoppedEvent;
		CSCore::EventConnectionUPtr m_timerStoppedConnection;
		CSCore::Timer m_gameTimer;
		
		std::ostringstream m_metricsOSS;
		bool m_isOutputInitialized = false;

        u32 m_currentParticles = 0;
		u32 m_runNum = 0;
		u32 m_leftSideHit = 0;
		u32 m_rightSideHit = 0;
		u32 m_renderBallCalled = 0;

        const u32 k_minParticles;
        const u32 k_maxParticles;
        const u32 k_particlesStep;
		const u32 k_maxRunNum;
		const u32 k_runTime;
	};
}

#endif