#include "BodyPart.h"


BBIT::BodyPart::BodyPart(Controlador &controller, BBIT::PART_ID id) 
	: controller(controller), 
	  identifier(id)
{
}

int  BBIT::BodyPart::getPartArea()
{
	return pixelArea;
}
void BBIT::BodyPart::reassignController(Controlador &controller)
{
	this->controller = controller;
}
void BBIT::BodyPart::createPointMap(std::vector<Ponto> textureVertices)
{
	
}
Ponto* BBIT::BodyPart::getNearestPoint(uint16_t x, uint16_t y)
{
	Ponto* point;
	int nearestPointIndex = controller.pontos->buscarPontoMaisProximo(x, y);;
	point = controller.pontos->pontos.at(nearestPointIndex);
	return point;
}

void BBIT::BodyPart::assignBoundaryPoint(uint16_t x, uint16_t y)
{
	std::tuple<int, int> tempCoords(x, y);
	selectedPoints.emplace_back(tempCoords);
}

BBIT::BodyPart::~BodyPart()
{
}

// 7th 1:30