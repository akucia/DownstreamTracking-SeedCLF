################################################################################
# Package: PatAlgorithms
################################################################################
gaudi_subdir(PatAlgorithms v5r1)

gaudi_depends_on_subdirs(Det/OTDet
                         Det/STDet
                         Event/RecEvent
                         Event/TrackEvent
                         Kernel/PartProp
                         OT/OTDAQ
			lwtnn
                         Tf/PatKernel
                         Tf/TfKernel
                         Tr/TrackInterfaces
                         Tr/TrackTools
		
			 
)

find_package(Boost)
find_package(ROOT)

include_directories(SYSTEM 
${Boost_INCLUDE_DIRS} 
${ROOT_INCLUDE_DIRS} 
)
gaudi_add_module(PatAlgorithms
                 src/*.cpp
                 INCLUDE_DIRS OT/OTDAQ Tf/PatKernel Tf/TfKernel Tr/TrackInterfaces lwtnn/include
                 LINK_LIBRARIES OTDetLib STDetLib RecEvent TrackEvent PartPropLib lwtnn)

gaudi_install_python_modules()

