// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/arc/notification/arc_notification_surface_impl.h"

#include "base/logging.h"
#include "components/exo/notification_surface.h"
#include "components/exo/surface.h"
#include "ui/aura/window.h"
#include "ui/views/controls/native/native_view_host.h"

namespace arc {

// Handles notification surface role of a given surface.
ArcNotificationSurfaceImpl::ArcNotificationSurfaceImpl(
    exo::NotificationSurface* surface)
    : surface_(surface) {
  DCHECK(surface);
}

gfx::Size ArcNotificationSurfaceImpl::GetSize() const {
  return surface_->GetContentSize();
}

void ArcNotificationSurfaceImpl::Attach(
    views::NativeViewHost* native_view_host) {
  DCHECK(!native_view_host_);
  DCHECK(native_view_host);
  native_view_host_ = native_view_host;
  native_view_host->Attach(surface_->host_window());
}

void ArcNotificationSurfaceImpl::Detach() {
  DCHECK(native_view_host_);
  DCHECK_EQ(surface_->host_window(), native_view_host_->native_view());
  native_view_host_->Detach();
  native_view_host_ = nullptr;
}

bool ArcNotificationSurfaceImpl::IsAttached() const {
  return native_view_host_;
}

views::NativeViewHost* ArcNotificationSurfaceImpl::GetAttachedHost() const {
  return native_view_host_;
}

aura::Window* ArcNotificationSurfaceImpl::GetWindow() const {
  return surface_->host_window();
}

aura::Window* ArcNotificationSurfaceImpl::GetContentWindow() const {
  DCHECK(surface_->host_window());
  DCHECK(!surface_->host_window()->children().empty());

  aura::Window* content_window = surface_->host_window()->children()[0];

  DCHECK(!content_window);
  return content_window;
}

const std::string& ArcNotificationSurfaceImpl::GetNotificationKey() const {
  return surface_->notification_key();
}

void ArcNotificationSurfaceImpl::FocusSurfaceWindow() {
  DCHECK(surface_->root_surface());
  DCHECK(surface_->root_surface()->window());

  // Focus the surface window manually to handle key events for notification.
  // Message center is unactivatable by default, but we make it activatable when
  // user is about to use Direct Reply. In that case, we also need to focus the
  // surface window manually to send events to Android.
  return surface_->root_surface()->window()->Focus();
}

}  // namespace arc
