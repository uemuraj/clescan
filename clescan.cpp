#include <windows.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Devices::Bluetooth;
using namespace winrt::Windows::Devices::Bluetooth::Advertisement;


struct bluetooth_addr
{
	const std::uint64_t value;

	bluetooth_addr(uint64_t addr) : value(addr)
	{}
};

std::wostream & operator<<(std::wostream & out, const std::uint8_t value)
{
	out << L"0123456789ABCDEF"[value / 16];
	out << L"0123456789ABCDEF"[value % 16];
	return out;
}

std::wostream & operator<<(std::wostream & out, const bluetooth_addr & addr)
{
	out << L'"';
	out << static_cast<std::uint8_t>((addr.value >> (5 * 8)) & 0xFF) << L':';
	out << static_cast<std::uint8_t>((addr.value >> (4 * 8)) & 0xFF) << L':';
	out << static_cast<std::uint8_t>((addr.value >> (3 * 8)) & 0xFF) << L':';
	out << static_cast<std::uint8_t>((addr.value >> (2 * 8)) & 0xFF) << L':';
	out << static_cast<std::uint8_t>((addr.value >> (1 * 8)) & 0xFF) << L':';
	out << static_cast<std::uint8_t>((addr.value >> (0 * 8)) & 0xFF);
	out << L'"';
	return out;
}

std::wostream & operator<<(std::wostream & out, BluetoothAddressType type)
{
	switch (type)
	{
	case BluetoothAddressType::Public:
		return out << L"Public";
	case BluetoothAddressType::Random:
		return out << L"Random";
	case BluetoothAddressType::Unspecified:
		return out << L"Unspecified";
	default:
		return out << std::dec << static_cast<int>(type);
	}
}

std::wostream & operator<<(std::wostream & out, BluetoothLEAdvertisementType type)
{
	switch (type)
	{
	case BluetoothLEAdvertisementType::ConnectableUndirected:
		return out << L"ConnectableUndirected";
	case BluetoothLEAdvertisementType::ConnectableDirected:
		return out << L"ConnectableDirected";
	case BluetoothLEAdvertisementType::ScannableUndirected:
		return out << L"ScannableUndirected";
	case BluetoothLEAdvertisementType::NonConnectableUndirected:
		return out << L"NonConnectableUndirected";
	case BluetoothLEAdvertisementType::ScanResponse:
		return out << L"ScanResponse";
	default:
		return out << std::dec << static_cast<int>(type);
	}
}

std::wostream & operator<<(std::wostream & out, BluetoothLEAdvertisementFlags flags)
{
	out << L"[ ";

	if ((flags & BluetoothLEAdvertisementFlags::LimitedDiscoverableMode) != BluetoothLEAdvertisementFlags::None)
	{
		out << L"LimitedDiscoverableMode,";
	}
	if ((flags & BluetoothLEAdvertisementFlags::GeneralDiscoverableMode) != BluetoothLEAdvertisementFlags::None)
	{
		out << L"GeneralDiscoverableMode,";
	}
	if ((flags & BluetoothLEAdvertisementFlags::ClassicNotSupported) != BluetoothLEAdvertisementFlags::None)
	{
		out << L"ClassicNotSupported,";
	}
	if ((flags & BluetoothLEAdvertisementFlags::DualModeControllerCapable) != BluetoothLEAdvertisementFlags::None)
	{
		out << L"DualModeControllerCapable,";
	}
	if ((flags & BluetoothLEAdvertisementFlags::DualModeHostCapable) != BluetoothLEAdvertisementFlags::None)
	{
		out << L"DualModeHostCapable,";
	}

	out << L" ]";

	return out;
}

std::wostream & operator<<(std::wostream & out, const winrt::hstring & str)
{
	if (str.empty())
		return out << L"null";

	return out << str.c_str();
}

std::wostream & operator<<(std::wostream & out, const winrt::guid & guid)
{
	wchar_t buff[50]{};

	if (::StringFromGUID2(guid, buff, _countof(buff)) > 0)
		out << L'"' << buff << L'"';

	return out;
}

std::wostream & operator<<(std::wostream & out, const IBuffer & buff)
{
	auto p = buff.data();
	auto q = p + buff.Length();

	while (p < q)
	{
		out << *p++;
	}

	return out;
}

std::wostream & operator<<(std::wostream & out, const DateTime & time)
{
	auto tt = winrt::clock::to_time_t(time);
	std::tm tm{};

	if (::localtime_s(&tm, &tt) == 0)
		out << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S %z");

	return out;
}

std::wostream & operator<<(std::wostream & out, const BluetoothLEAdvertisementDataSection & data)
{
	return out << L"{ DataType: " << data.DataType() << L", Data: " << data.Data() << L" }";
}

std::wostream & operator<<(std::wostream & out, const BluetoothLEManufacturerData & data)
{
	return out << L"{ CompanyId: " << data.CompanyId() << L", Data: " << data.Data() << L" }";
}

template<typename T>
std::wostream & operator<<(std::wostream & out, const IReference<T> & ref)
{
	return (ref == nullptr) ? out << L"null" : out << ref.Value();
}

template<typename T>
std::wostream & operator<<(std::wostream & out, const IVector<T> & vector)
{
	out << L"[ ";

	for (decltype(vector.Size()) index{}; index < vector.Size(); ++index)
	{
		if (index == 0)
			out << vector.GetAt(index);
		else
			out << L", " << vector.GetAt(index);
	}

	out << L" ]";

	return out;
}

std::wostream & operator<<(std::wostream & out, const BluetoothLEAdvertisementReceivedEventArgs & args)
{
	const auto & advertisement = args.Advertisement();

	out << L"BluetoothLEAdvertisement:" << std::endl;
	out << L"  AddressType: " << args.BluetoothAddressType() << std::endl;
	out << L"  Address: " << bluetooth_addr(args.BluetoothAddress()) << std::endl;
	out << L"  AdvertisementType: " << args.AdvertisementType() << std::endl;
	out << L"  Advertisement:" << std::endl;
	out << L"    LocalName: " << advertisement.LocalName() << std::endl;
	out << L"    Flags: " << advertisement.Flags() << std::endl;
	out << L"    ServiceUuids: " << advertisement.ServiceUuids() << std::endl;
	out << L"    DataSections: " << advertisement.DataSections() << std::endl;
	out << L"    ManufacturerData: " << advertisement.ManufacturerData() << std::endl;
	out << L"  RawSignalStrengthInDBm: " << args.RawSignalStrengthInDBm() << std::endl;
	out << L"  TransmitPowerLevelInDBm: " << args.TransmitPowerLevelInDBm() << std::endl;
	out << L"  Timestamp: " << args.Timestamp();

	return out;
}
