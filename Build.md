export VULKAN_SDK=/Users/ogalizzi/VulkanSDK/1.3.268.1

export VK_ICD_FILENAMES=$VULKAN_SDK/MoltenVK/dylib/macOS/MoltenVK_icd.json

export DYLD_LIBRARY_PATH=$VULKAN_SDK/MoltenVK/dylib/macOS/MoltenVK_icd.json:DYLD_LIBRARY_PATH

export DYLD_LIBRARY_PATH=$VULKAN_SDK/macOS/lib/:$DYLD_LIBRARY_PATH


brew install vulkan-tools
brew install libvulkan-dev
  sudo apt install vulkan-validationlayers 

brew install spirv-tools
