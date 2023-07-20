//
// GstVideoControl.xaml.cpp
// Implementation of the GstVideoControl class
//

#include "pch.h"
#include "GstVideoControl.xaml.h"
#include "GstWrapper.h"
#include <gst/video/video.h>

using namespace CppControls;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

GstVideoControl::GstVideoControl()
{
	InitializeComponent();
}

void GstVideoControl::btnStart_Click(Object^ sender, RoutedEventArgs^ e)
{
    auto gstHelper = GstWrapper::Instance::get();
    startPipeline();
    updateUIElements();
}

void GstVideoControl::btnStop_Click(Object^ sender, RoutedEventArgs^ e)
{
    stopPipeline();
    updateUIElements();
}

bool GstVideoControl::startPipeline()
{
    stopPipeline();

    pipeline_ = gst_parse_launch(
        "videotestsrc ! queue ! d3d11videosink name=overlay", NULL);
    GstElement* overlay = gst_bin_get_by_name(GST_BIN(pipeline_), "overlay");

    if (overlay) {
        gst_video_overlay_set_window_handle(
            GST_VIDEO_OVERLAY(overlay),
            (guintptr) reinterpret_cast<IUnknown*>(GstVideoControl_VideoPanel));
        gst_object_unref(overlay);
    }

    gst_element_set_state(pipeline_, GST_STATE_PLAYING);

    GstStateChangeReturn ret;
    ret = gst_element_set_state(GST_ELEMENT(pipeline_), GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        stopPipeline();
        return false;
    }

    GstVideoControl_VideoPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;

    isPlaying_ = true;

    return true;
}

bool GstVideoControl::stopPipeline()
{
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_NULL);
        gst_clear_object(&pipeline_);
    }

    isPlaying_ = false;

    return true;
}

void GstVideoControl::updateUIElements()
{
    btnStart->IsEnabled = !isPlaying_;
    btnStop->IsEnabled = isPlaying_;
}
