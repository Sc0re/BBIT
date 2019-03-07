#pragma once

#include <map>
#include <cmath>
#include <vector>

#include "code/Pontos.h"
#include "code/Controlador.h"

namespace BBIT
{
	enum class PART_ID
	{
		ARM_UPPER_RIGHT,
		ARM_FOREARM_RIGHT,
		ARM_UPPER_LEFT,
		ARM_FOREARM_LEFT,
		BREAST,
		WAIST,
		HIPS,
		THIGH_RIGHT,
		THIGH_LEFT,
		CALF_RIGHT,
		CALF_LEFT
	};

	class BodyPart
	{
	public:
		BodyPart(Controlador &controller, BBIT::PART_ID id);
		virtual ~BodyPart();
		int getPartArea();
		
		void createPointMap(std::vector<Ponto> textureVertices);
		Ponto* getNearestPoint(uint16_t x, uint16_t y);
		void assignBoundaryPoint(uint16_t x, uint16_t y);

	private:
		BBIT::PART_ID identifier;
		Controlador &controller;
		/*
			pixelArea should be generated incrementally as image modification
			happens. Since the area should be increased in increments up to the
			point where it meets the required change in area specified by the
			buttons (+/- 5%), this should be updated each tick.
		*/
		int pixelArea;
		/*
			pointMap is the mapping of points from the original set created via
			marching squares in Diego's original implementation to control points
			created here and linked based on distance.
		*/
		std::map<Ponto, Ponto> pointMap;
		int partArea;
		std::vector<Ponto> distalPoints, proximalPoints;
		std::vector<tuple<int, int>> selectedPoints;
		void reassignController(Controlador &controller);
	};
}