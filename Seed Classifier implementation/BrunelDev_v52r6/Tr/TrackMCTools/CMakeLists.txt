################################################################################
# Package: TrackMCTools
################################################################################
gaudi_subdir(TrackMCTools v3r13)

gaudi_depends_on_subdirs(Det/OTDet
                         Det/STDet
                         Det/VeloDet
                         Event/LinkerEvent
                         Event/MCEvent
                         Event/RecEvent
                         Event/TrackEvent
                         GaudiAlg
                         Kernel/MCInterfaces
                         Tf/PatKernel
                         Tr/TrackInterfaces)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(TrackMCTools
                 src/*.cpp
                 INCLUDE_DIRS Kernel/MCInterfaces Tf/PatKernel Tr/TrackInterfaces
                 LINK_LIBRARIES OTDetLib STDetLib VeloDetLib LinkerEvent MCEvent RecEvent TrackEvent GaudiAlgLib)

