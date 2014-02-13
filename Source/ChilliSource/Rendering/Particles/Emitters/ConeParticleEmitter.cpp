//
//  ConeParticleEmitter.cpp
//  MoshiMonsters
//
//  Created by Andrew Glass on 02/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Emitters/ConeParticleEmitter.h>

#include <ChilliSource/Core/Math/MathUtils.h>

namespace moFlo
{
    namespace Rendering
    {
        //-------------------------------------------------------------
        /// Constructor
        ///
        /// @param Param Dictionary
        //-------------------------------------------------------------
        CConeParticleEmitter::CConeParticleEmitter(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent)
        :CParticleEmitter(inParams, inpMaterial, inpComponent)
        {
            std::string strTemp;
            
            f32 fRadius = 45.0f;
            //Radius
            if(inParams.TryGetValue("Radius", strTemp))
            {
                fRadius =Core::CMathUtils::Clamp( Core::CStringConverter::ParseFloat(strTemp), 1.0f, 89.0f);
            }
            
            mfBoxArea = sinf(Core::CMathUtils::DegToRad(fRadius));
            mfBoxHeight = cosf(Core::CMathUtils::DegToRad(fRadius));
            
        }
        //-----------------------------------------------------
        /// Create
        ///
        /// Create a new emitter from the given parameters
        ///
        /// @param Param dictionary
        /// @param Material
        /// @param Owning system
        /// @return Ownership of point emitter
        //-----------------------------------------------------
        CParticleEmitter* CConeParticleEmitter::Create(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent)
        {
            return new CConeParticleEmitter(inParams, inpMaterial, inpComponent);
        }
		//-----------------------------------------------------
		/// Emit
		///
		/// Emit some new particles by pulling from the dead
		/// pool. Emission will depend on the number of
		/// particles emitted and the shape of the emitter
		///
		/// @param Particles
		/// @param Index of current particle
		//-----------------------------------------------------
		void CConeParticleEmitter::Emit(Particle* inpParticles, u32 udwParticleIndex)
        {
            f32 fBoxAreaSq = mfBoxArea * mfBoxArea;
            Core::CVector3 vDir;
            do
            {
                vDir.x = Core::CMathUtils::RandomInRange(-mfBoxArea, mfBoxArea);
                vDir.z = Core::CMathUtils::RandomInRange(-mfBoxArea, mfBoxArea);
            }while(fBoxAreaSq < (vDir.x *vDir.x + vDir.z *vDir.z));
            
            vDir.y = mfBoxHeight;
            inpParticles->vVelocity[udwParticleIndex] = vDir;
			inpParticles->vVelocity[udwParticleIndex].Normalise();
            inpParticles->vVelocity[udwParticleIndex] *= mfInitialVelocity;
        }
        
    }
}