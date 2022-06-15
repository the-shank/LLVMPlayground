option(USE_REFERENCE "Build with reference solution" OFF)

include_directories(include)

if (USE_REFERENCE)
  message(STATUS "Use reference solution")
  add_library(DataflowPass MODULE
    src/DataflowAnalysis.cpp
    src/PointerAnalysis.cpp
    src/DivZeroAnalysis.cpp
  )

  target_link_libraries(DataflowPass RefDomain)

  add_library(RefDomain MODULE
    reference/RefDomain.cpp
    )
else (USE_REFERENCE)
  add_library(DataflowPass MODULE
  src/DataflowAnalysis.cpp
  src/PointerAnalysis.cpp
  src/DivZeroAnalysis.cpp
  src/Domain.cpp
  )
endif (USE_REFERENCE)

target_compile_features(DataflowPass PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(DataflowPass PROPERTIES
        COMPILE_FLAGS "-g"
        )

if(APPLE)
    set_target_properties(DataflowPass PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)
