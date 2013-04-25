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

#ifndef DETOURSEPARATIONBEHAVIOR_H
#define DETOURSEPARATIONBEHAVIOR_H

#include "DetourSteeringBehavior.h"

struct dtCrowdAgent;
class dtCrowd;


/// Implementation of the separation behavior.
///
/// An agent using this behavior will try to keep its distance from one or more targets.
/// The minimum distance to keep from the targets can be specified.
class dtSeparationBehavior : public dtSteeringBehavior
{
public:
	dtSeparationBehavior();
	~dtSeparationBehavior();

	virtual void update(dtCrowdAgent* oldAgent, dtCrowdAgent* newAgent, float dt);
	virtual void computeForce(const dtCrowdAgent* ag, float* force);
};

#endif // DETOURSEPARATIONBEHAVIOR_H