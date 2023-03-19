#import "vulkan_helper_macos.h"
#import <QuartzCore/QuartzCore.h>
#import <Cocoa/Cocoa.h>

int assignMetalLayer(void* view) {
  ((NSView*)view).layer = [CAMetalLayer layer];
  return 1;
}




