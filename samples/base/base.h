#pragma once

#include "clan_sample.h"

class BaseSample : public clan::ClanSample
{
  public:
	BaseSample() = default;

	virtual ~BaseSample() = default;

	virtual bool prepare(clan::Platform &platform) override;

  private:

	virtual void draw_gui() override;
};

std::unique_ptr<clan::ClanSample> create_base();