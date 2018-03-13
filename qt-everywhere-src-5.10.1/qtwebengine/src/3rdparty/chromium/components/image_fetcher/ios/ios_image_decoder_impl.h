// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_IMAGE_FETCHER_IOS_IMAGE_DECODER_IMPL_H_
#define COMPONENTS_IMAGE_FETCHER_IOS_IMAGE_DECODER_IMPL_H_

#include "base/memory/ref_counted.h"
#include "components/image_fetcher/core/image_decoder.h"

namespace base {
class TaskRunner;
}

namespace image_fetcher {

// Factory for iOS specific implementation of ImageDecoder.
std::unique_ptr<ImageDecoder> CreateIOSImageDecoder(
    scoped_refptr<base::TaskRunner> task_runner);

}  // namespace image_fetcher

#endif  // COMPONENTS_IMAGE_FETCHER_IOS_IMAGE_DECODER_IMPL_H_