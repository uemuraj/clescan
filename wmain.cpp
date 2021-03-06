#include <windows.h>

#include <locale>
#include <iostream>
#include <sstream>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>

using namespace winrt;
using namespace winrt::Windows::Devices::Bluetooth::Advertisement;

std::wostream & operator<<(std::wostream & out, const BluetoothLEAdvertisementReceivedEventArgs & args);

int wmain(int argc, wchar_t * argv[], wchar_t * envp[])
{
	std::locale::global(std::locale(""));

	init_apartment();

	BluetoothLEAdvertisementWatcher watcher;
	watcher.ScanningMode(BluetoothLEScanningMode::Active);
	watcher.AllowExtendedAdvertisements(true);
	watcher.Received([=](auto, auto args) {
	#if 0
		std::wstringstream wss;
		wss << L"---" << std::endl << args << std::endl;
		::OutputDebugString(wss.str().c_str());
	#else
		std::wcout << L"---" << std::endl << args << std::endl;
	#endif
		}
	);

	std::wcerr << L"Hit enter to stop." << std::endl;
	watcher.Start();

	std::cin.ignore();
	watcher.Stop();
}
