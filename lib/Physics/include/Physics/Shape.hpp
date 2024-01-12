#pragma once

namespace Gaze::Physics {
	class Shape
	{
		struct Impl;

	public:
		Shape();
		virtual ~Shape();

		[[nodiscard]] auto Handle() const noexcept -> void*;

	protected:
		Impl* m_pImpl;
	};

	class BoxShape : public Shape
	{
	public:
		BoxShape(float width, float height, float depth);
	};
}
