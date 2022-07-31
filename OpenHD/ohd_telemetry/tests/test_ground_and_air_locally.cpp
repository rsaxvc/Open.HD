//
// For testing, run the air and ground telemetry services side by side on the same machine locally.
//
#include "../src/OHDTelemetry.hpp"
#include "openhd-platform.hpp"
#include "openhd-profile.hpp"
#include "openhd-platform-discover.hpp"

#include <iostream>
#include <memory>
#include <thread>

int main() {
  std::cout<<"start\n";
  std::unique_ptr<OHDTelemetry> ohdTelemGround;
  std::unique_ptr<OHDTelemetry> ohdTelemAir;
  {
	OHDProfile profile{false, "XX"};
        //OHDPlatform platform{PlatformType::PC};
        const auto platform=DPlatform::discover();
	ohdTelemGround = std::make_unique<OHDTelemetry>(*platform, profile);
        auto example_comp=std::make_shared<openhd::testing::DummyGroundXSettingsComponent>();
        // MAV_COMP_ID_ONBOARD_COMPUTER2=192
        ohdTelemGround->add_settings_generic(example_comp->get_all_settings());
  }
  {
	OHDProfile profile{true, "XX"};
	//OHDPlatform platform{PlatformType::PC};
        const auto platform=DPlatform::discover();
	ohdTelemAir = std::make_unique<OHDTelemetry>(*platform, profile);
        auto example_comp=std::make_shared<openhd::testing::DummyCameraXSettingsComponent>();
        ohdTelemAir->add_settings_generic(example_comp->get_all_settings());
  }
  while (true) {
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::stringstream ss;
	ss<<"G and air debug:\n";
	ss<<ohdTelemGround->createDebug();
	ss<<ohdTelemAir->createDebug();
	std::cout<<ss.str();
  }
}

