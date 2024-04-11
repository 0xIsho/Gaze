#pragma once

#include "Core/Type.hpp"

struct ServerPacket
{
	Gaze::I32 playerPos[2];
	Gaze::Byte playerDir[2];
};

struct ClientPacket
{
	Gaze::Byte left;
	Gaze::Byte right;
};

static constexpr auto kWindowWidth = 1200;
static constexpr auto kWindowHeight = 1024;
static constexpr auto kPaddleWidth = 100;
static constexpr auto kPaddleHeight = 10;
static constexpr auto kPaddleBottomOffset = 50;
static constexpr auto kPaddleSpeed = 400;
