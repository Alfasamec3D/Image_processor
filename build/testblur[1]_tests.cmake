add_test([=[ImageProcessingTest.BlurConversion]=]  /home/alfasamec3d/media/programs/Image_processor/build/testblur [==[--gtest_filter=ImageProcessingTest.BlurConversion]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ImageProcessingTest.BlurConversion]=]  PROPERTIES WORKING_DIRECTORY /home/alfasamec3d/media/programs/Image_processor/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  testblur_TESTS ImageProcessingTest.BlurConversion)
