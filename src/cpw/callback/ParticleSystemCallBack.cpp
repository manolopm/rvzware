/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnológico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
*/
#include <math.h>

//#include <stdafx.h>

#include <cpw/callback/ParticleSystemCallBack.h>
#include <cpw/graphic/ApplicationScene.h>
#include <cpw/graphic/IScene.h>
#include <cpw/entity/ElementParticleSystem.h>


#define PSC_MULT 0.016f;
//#define PSC_MULT 0.02f;

using namespace cpw;

ParticleSystemCallBack::ParticleSystemCallBack()
{

}

ParticleSystemCallBack::~ParticleSystemCallBack()
{

}

void ParticleSystemCallBack::Insert(const cpw::Callable* callable)
{
	cpw::ElementParticleSystem *entity = (cpw::ElementParticleSystem *) callable;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	std::ostringstream parent_id_ss, entity_id_ss;
	std::string parent_id, entity_id;

	parent_id_ss << entity->GetParent()->GetId();
	parent_id = parent_id_ss.str();

	entity_id_ss << entity->GetId();
	entity_id = entity_id_ss.str();
	
	if(entity->GetPolygonal()) //polygonal fire
	{
		//app_scene->AddMultiLineParticleSystem(entity->GetCoords(),
		app_scene->AddMultiLineParticleSystem(entity->GetVertices(),
												entity->GetTextureFileName(),
												entity_id,
												parent_id,
												entity->GetNewParticlesSec(),
												entity->GetParticlesLife(),
												entity->GetAlpha(),
												entity->GetSenoidalInterpolator(),
												entity->GetAdditiveBlend(),
												entity->GetSpeed(), 
												entity->GetPhi(), 
												entity->GetTheta(),
												entity->GetSize(),
												entity->GetMaxPerimeterRadius());

		

		/*for (int i=1; i<entity->GetVertexCount(); i++)
		{
			cpw::Point3d<float> new_vertex;
			if (entity->GetVertex(i, new_vertex))
			{
				app_scene->AddVertexToMultiLineParticleSystem(new_vertex, entity_id );
			}
		}*/

		//update the new particles emission rate

		//compute the length of the perimeter
		float total_length = 0.0f;
		cpw::Point3d<float> v1, v2, v3;
		for (int i=0; i<(entity->GetNumVertices()-1); i++)
		{
			entity->GetVertex(i, v1);
			entity->GetVertex(i+1, v2);

			if (v1 != v2)
			{
				v3.x = v2.x - v1.x;
				v3.y = v2.y - v1.y;
				v3.z = v2.z - v1.z;

				total_length += sqrt((v3.x * v3.x) + (v3.y * v3.y) + (v3.z * v3.z));
			}
		}




		cpw::Range<int> new_rate;
		if (entity->GetNumVertices() == 1)
		{
			new_rate.min = entity->GetNewParticlesSec().min;
			new_rate.max = entity->GetNewParticlesSec().max;
		}
		else
		{
			new_rate.min = entity->GetNewParticlesSec().min * total_length * PSC_MULT;
			new_rate.max = entity->GetNewParticlesSec().max * total_length * PSC_MULT;
		}
		
		app_scene->UpdateParticleSystemParticleGeneratorRate(entity_id, new_rate);
	}
	else //puntual fire
	{
		app_scene->AddPuntualParticleSystem(entity->GetCoords(),
											entity->GetTextureFileName(),
											entity_id,
											parent_id,
											entity->GetNewParticlesSec(),
											entity->GetParticlesLife(),
											entity->GetAlpha(),
											entity->GetSenoidalInterpolator(),
											entity->GetAdditiveBlend(),
											entity->GetSpeed(), 
											entity->GetPhi(), 
											entity->GetTheta(),
											entity->GetSize());
	}

	
}



void ParticleSystemCallBack::Update(cpw::Callable* callable)
{
	cpw::ElementParticleSystem *entity = (cpw::ElementParticleSystem *) callable;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	std::ostringstream parent_id_ss, entity_id_ss;
	std::string parent_id, entity_id;

	parent_id_ss << entity->GetParent()->GetId();
	parent_id = parent_id_ss.str();

	entity_id_ss << entity->GetId();
	entity_id = entity_id_ss.str();

	if(entity->GetPolygonal()) //polygonal fire
	{
		/*
		
		app_scene->AddMultiLineParticleSystem(entity->GetCoords(),
												entity->GetTextureFileName(),
												entity_id,
												parent_id,
												entity->GetNewParticlesSec(),
												entity->GetParticlesLife(),
												entity->GetAlpha(),
												entity->GetSenoidalInterpolator(),
												entity->GetAdditiveBlend(),
												entity->GetSpeed(), 
												entity->GetPhi(), 
												entity->GetTheta(),
												entity->GetSize());

		

		for (int i=1; i<entity->GetVertexCount(); i++)
		{
			cpw::Point3d<float> new_vertex;
			if (entity->GetVertex(i, new_vertex))
			{
				app_scene->AddVertexToMultiLineParticleSystem(new_vertex, entity_id );
			}
		}

		//update the new particles emission rate
		cpw::Range<int> new_rate;
		new_rate.min = entity->GetNewParticlesSec().min * entity->GetVertexCount();
		new_rate.max = entity->GetNewParticlesSec().max * entity->GetVertexCount();
		
		app_scene->UpdateParticleSystemParticleGeneratorRate(entity_id, new_rate);*/
	}
	else //puntual fire
	{
		app_scene->UpdatePuntualParticleSystem(entity->GetCoords(),
											   entity->GetTextureFileName(),
											   entity_id,
											   entity->GetNewParticlesSec(),
											   entity->GetParticlesLife(),
											   entity->GetAlpha(),
											   entity->GetSenoidalInterpolator(),
											   entity->GetAdditiveBlend(),
											   entity->GetSpeed(), 
											   entity->GetPhi(), 
											   entity->GetTheta(),
											   entity->GetSize());
	}
}

void ParticleSystemCallBack::Delete(cpw::Callable* callable)
{
	cpw::ElementParticleSystem *entity = (cpw::ElementParticleSystem *) callable;
	cpw::IScene *app_scene = cpw::ApplicationScene::GetInstance()->GetScene();
	
	std::ostringstream parent_id_ss, entity_id_ss;
	std::string parent_id, entity_id;

	if (entity->GetParent() == NULL)
	{
		//esto va a petar,porque no tiene padre cuando deberia tenerlo
		int error;
		error = 1;
		return;
	}
	parent_id_ss << entity->GetParent()->GetId();
	parent_id = parent_id_ss.str();

	entity_id_ss << entity->GetId();
	entity_id = entity_id_ss.str();

	app_scene->DeleteParticleSystemFromScene(entity_id, parent_id);
}


ParticleSystemCallBack::ParticleSystemCallBack(const ParticleSystemCallBack &pscb)
{
}

ParticleSystemCallBack &ParticleSystemCallBack::operator = (const ParticleSystemCallBack &pscb)
{
	return *this;
}

ICallBack* ParticleSystemCallBack::Clone() 
{
	return new ParticleSystemCallBack(*this);
}

void ParticleSystemCallBack::Animate(cpw::Callable* callable, cpw::cpwTime &time)
{
}

void ParticleSystemCallBack::Visualize(cpw::Callable* callable, const bool &value)
{
}