#pragma once
#include "pxr_base/usb/libusb_header.h"
#include "pxr_base/usb/usb_common_define.h"

#include <functional>

namespace pxr_base
{
	class UsbDeviceConfigDescriptor
	{
	public:
		UsbDeviceConfigDescriptor(libusb_device* device);
		~UsbDeviceConfigDescriptor();

		bool GetEndpointAddress(UsbTransferType type, UsbTransferDirection direction,
			uint8_t& endpnt);
		bool GetVendorInterfaceNum(uint8_t sub_class, uint8_t& num);
	private:
		bool ForEachInterface(const libusb_config_descriptor* config_desp,
			std::function<bool(const libusb_interface&)> f);

		bool ForEachInterfaceDescriptor(const libusb_interface* interface,
			std::function<bool(const libusb_interface_descriptor&)> f);

		bool ForEachEndpoint(const libusb_interface_descriptor* inter_desp,
			std::function<bool(const libusb_endpoint_descriptor&)> f);

		bool GetEndpointAddress(const libusb_interface* interface,
			uint8_t type, uint8_t direction, uint8_t& endpnt);
		bool GetEndpointAddress(const libusb_interface_descriptor* interface_desp,
			uint8_t type, uint8_t direction, uint8_t& endpnt);
	private:
		libusb_config_descriptor* config_desp_ = nullptr;
	};
}