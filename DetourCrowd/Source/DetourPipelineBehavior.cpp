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

#include "DetourPipelineBehavior.h"

#include "DetourAlloc.h"
#include "DetourCrowd.h"

#include <new>


dtPipelineBehavior::dtPipelineBehavior()
	: dtBehavior()
	, m_behaviors(0)
	, m_nbBehaviors(0)
{
}

dtPipelineBehavior::~dtPipelineBehavior()
{
}

dtPipelineBehavior* dtPipelineBehavior::allocate()
{
	void* mem = dtAlloc(sizeof(dtPipelineBehavior), DT_ALLOC_PERM);

	if (mem)
		return new(mem) dtPipelineBehavior();

	return 0;
}

void dtPipelineBehavior::free(dtPipelineBehavior* ptr)
{
	if (!ptr)
		return;

	ptr->~dtPipelineBehavior();
	dtFree(ptr);
	ptr = 0;
}

void dtPipelineBehavior::update(const dtCrowdQuery& query, const dtCrowdAgent& oldAgent, dtCrowdAgent& newAgent, float dt)
{
	if (m_behaviors == 0 || m_nbBehaviors == 0)
		return;

	recursiveUpdate(query, oldAgent, newAgent, dt, m_nbBehaviors);
}

void dtPipelineBehavior::recursiveUpdate(const dtCrowdQuery& query, const dtCrowdAgent& oldAgent, dtCrowdAgent& newAgent, float dt, unsigned remainingBehaviors)
{
	if (remainingBehaviors == 0)
		return;

	m_behaviors[m_nbBehaviors - remainingBehaviors]->update(query, oldAgent, newAgent, dt);
	dtCrowdAgent ag = newAgent;

	recursiveUpdate(query, newAgent, ag, dt, remainingBehaviors - 1);
	newAgent = ag;
}

void dtPipelineBehavior::setBehaviors(dtBehavior** behaviors, unsigned nbBehaviors)
{
	m_behaviors = behaviors;
	m_nbBehaviors = nbBehaviors;
}