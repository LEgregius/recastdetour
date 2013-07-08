//
// Copyright (c) 2013 MASA Group recastdetour@masagroup.net
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef DETOURPIPELINETEST_H
#define DETOURPIPELINETEST_H

#define CATCH_CONFIG_MAIN

#ifdef _MSC_VER
#pragma warning(push, 0)
#include <catch.hpp>
#pragma warning(pop)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic pop
#endif

#include "DetourCrowdTestUtils.h"

#include "DetourParametrizedBehavior.h"
#include "DetourAlignmentBehavior.h"
#include "DetourCohesionBehavior.h"
#include "DetourCommon.h"
#include "DetourFlockingBehavior.h"
#include "DetourGoToBehavior.h"
#include "DetourPathFollowing.h"
#include "DetourPipelineBehavior.h"
#include "DetourSeekBehavior.h"
#include "DetourSeparationBehavior.h"


TEST_CASE("DetourCrowdTest/Pipeline", "Tests about the pipeline behavior")
{
	TestScene ts;
	dtCrowd* crowd = ts.createSquareScene(20, 0.5);

	REQUIRE(crowd != 0);

	SECTION("Adding and Removing behaviors to the pipeline", "Trying to add and remove behaviors into the pipeline, should not crash")
	{
		dtPipelineBehavior* pipeline = dtPipelineBehavior::allocate();

		float posAgt1[] = {0, 0.2f, 0};
		float destAgt1[] = {15, 0, 0};

		dtCrowdAgent ag;

		// Adding the agents to the crowd
		REQUIRE(crowd->addAgent(ag, posAgt1));
		ts.defaultInitializeAgent(*crowd, ag.id);

		crowd->setAgentBehavior(ag.id, pipeline);

		crowd->update(0.5, 0);

		float agt1NewPos[3];
		dtVcopy(agt1NewPos, crowd->getAgent(ag.id)->position);

		// Since no behavior is affected to the pipeline, the agent must not have moved
		CHECK(dtVequal(agt1NewPos, posAgt1));

		dtPathFollowing* pf = dtPathFollowing::allocate(5);
		dtPathFollowingParams* pfParams = pf->getBehaviorParams(crowd->getAgent(0)->id);
		pfParams->init(256);

		// Set the destination
		dtPolyRef dest, firstPoly;
		float nearest[3], firstPos[3];
		crowd->getCrowdQuery()->getNavMeshQuery()->findNearestPoly(destAgt1, crowd->getCrowdQuery()->getQueryExtents(), crowd->getCrowdQuery()->getQueryFilter(), &dest, nearest);
		crowd->getCrowdQuery()->getNavMeshQuery()->findNearestPoly(posAgt1, crowd->getCrowdQuery()->getQueryExtents(), crowd->getCrowdQuery()->getQueryFilter(), &firstPoly, firstPos);
		pfParams->corridor.reset(firstPoly, firstPos);

		REQUIRE(dest != 0);		
		REQUIRE(pf->init(*crowd->getCrowdQuery()));
		REQUIRE(pf->requestMoveTarget(ag.id, dest, destAgt1));

		pipeline->setBehaviors(0, 1);

		// Still no behavior was given (null pointer), so nothing should have moved.
		crowd->update(0.5, 0);

		dtVcopy(agt1NewPos, crowd->getAgent(ag.id)->position);	

		// Since no behavior is affected to the pipeline, the agent must not have moved
		CHECK(dtVequal(agt1NewPos, posAgt1));

		dtBehavior* behavior = pf;

		// This time we affect the right behavior but with a size of 0
		pipeline->setBehaviors(&behavior, 0);

		crowd->update(0.5, 0);

		// The agent should not have moved
		dtVcopy(agt1NewPos, crowd->getAgent(ag.id)->position);	
		CHECK(dtVequal(agt1NewPos, posAgt1));

		// This time we affect the behavior with the right size
		pipeline->setBehaviors(&behavior, 1);

		crowd->update(0.5, 0);

		dtVcopy(agt1NewPos, crowd->getAgent(ag.id)->position);	

		// A behavior has been affected to the pipeline, the agent should have moved
		CHECK(!dtVequal(agt1NewPos, posAgt1));

		dtPathFollowing::free(pf);
		dtPipelineBehavior::free(pipeline);
	}
}

#endif