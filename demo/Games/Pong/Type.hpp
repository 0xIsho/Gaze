#pragma once

#include "Core/Type.hpp"

#include <glm/vec2.hpp>

struct ServerPacket
{
	Gaze::I32 p1Pos[2];
	Gaze::I32 p1Dir[2];
	Gaze::I32 p2Pos[2];
	Gaze::I32 p2Dir[2];
	Gaze::I32 ballPos[2];
	Gaze::I32 ballDir[2];
};

struct ClientPacket
{
	Gaze::Byte start;
	Gaze::Byte up;
	Gaze::Byte down;
};

static constexpr auto kWinWidth = 1200;
static constexpr auto kWinHeight = 600;
static constexpr auto kPaddleSize = glm::vec2(10.F, 50.F);
static constexpr auto kPaddleSpeed = 300.0F;
static constexpr auto kDefaultPaddleOffsetX = kPaddleSize.x * 2;
static constexpr auto kBallSize = 10.F;
static constexpr auto kBallSpeed = 500.F;
