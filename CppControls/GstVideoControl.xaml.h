//
// GstVideoControl.xaml.h
// Declaration of the GstVideoControl class
//

#pragma once

#include "GstVideoControl.g.h"

struct _GstElement;
typedef struct _GstElement GstElement;

namespace CppControls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class GstVideoControl sealed
	{
	public:
		GstVideoControl();
	private:
		void btnStart_Click(Platform::Object^ sender,
			Windows::UI::Xaml::RoutedEventArgs^ e);
		void btnStop_Click(Platform::Object^ sender,
			Windows::UI::Xaml::RoutedEventArgs^ e);

		bool startPipeline();
		bool stopPipeline();
		void updateUIElements();
	private:
		GstElement* pipeline_ = nullptr;
		bool isPlaying_ = false;
	};
}
