######################################################################
# Automatically generated by qmake (2.01a) ?? ????. 23 01:03:51 2012
######################################################################

TEMPLATE = app
TARGET = ImageProcessor
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += exceptions/Exception.hpp \
           exceptions/BadArgsException.hpp \
           exceptions/CustomFilterCreateException.hpp \
           Constants.hpp \
           Utils.hpp \
           ERgb.hpp \
           Arguments.hpp \
           Histogram.hpp \
           Algorithms.hpp \
           filters/Filter.hpp \
           filters/AutolevelsFilter.hpp \
           filters/AutocontrastFilter.hpp \
           filters/GrayscaleFilter.hpp \
           filters/GrayworldFilter.hpp \
           filters/MedianFilter.hpp \
           filters/GaussianFilter.hpp \
           filters/SobelFilter.hpp \
           filters/CustomFilter.hpp \
           filters/RotateFilter.hpp \
           filters/ResizeFilter.hpp \
           filters/WaveFilter.hpp \
           filters/GlassFilter.hpp \
           filters/GlowingEdgeFilter.hpp \
           filters/AverageSobelFilter.hpp \
           filters/SharpenFilter.hpp \
           filters/MaximumFilter.hpp \
           widgets/AutoLCFilterDialog.hpp \
           widgets/SobelFilterDialog.hpp \
           widgets/WaveFilterDialog.hpp \
           widgets/ImageView.hpp \
           widgets/MainWindow.hpp

SOURCES += Arguments.cpp \
           Algorithms.cpp \
           Histogram.cpp \
           filters/AutolevelsFilter.cpp \
           filters/AutocontrastFilter.cpp \
           filters/GrayscaleFilter.cpp \
           filters/GrayworldFilter.cpp \
           filters/MedianFilter.cpp \
           filters/GaussianFilter.cpp \
           filters/SobelFilter.cpp \
           filters/CustomFilter.cpp \
           filters/RotateFilter.cpp \
           filters/ResizeFilter.cpp \
           filters/WaveFilter.cpp \
           filters/GlassFilter.cpp \
           filters/GlowingEdgeFilter.cpp \
           filters/AverageSobelFilter.cpp \
           filters/SharpenFilter.cpp \
           filters/MaximumFilter.cpp \
           widgets/AutoLCFilterDialog.cpp \
           widgets/SobelFilterDialog.cpp \
           widgets/WaveFilterDialog.cpp \
           widgets/ImageView.cpp \
           widgets/MainWindow.cpp \
           main.cpp

RESOURCES += icons.qrc

CONFIG += qt warn_on release
