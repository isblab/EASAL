target_sources(easalqtgui
    PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/AboutWindow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/AdvancedOptionWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/AtlasViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/BondViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/InputWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/StatsWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/PseudoAtlasWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/CayleyStatisticsWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/Mesh3D.cpp
    	${CMAKE_CURRENT_LIST_DIR}/NodeParentChildernViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/SharedDataGUI.cpp
    	${CMAKE_CURRENT_LIST_DIR}/SweepViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/CayleySpaceViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/CustomSlider.cpp
    	${CMAKE_CURRENT_LIST_DIR}/Camera.cpp
    	${CMAKE_CURRENT_LIST_DIR}/Renderer.cpp
    	${CMAKE_CURRENT_LIST_DIR}/CayleyPointRenderingWrapper.cpp
    	${CMAKE_CURRENT_LIST_DIR}/RealisationViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/MainWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/RealizationViewWidgetCornerVersion.cpp
    	${CMAKE_CURRENT_LIST_DIR}/ConstraintSelectionWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/ConstraintSelectionMoleculeViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/ConstraintSelectionBondViewWidget.cpp
    	${CMAKE_CURRENT_LIST_DIR}/MessageWindow.cpp
    	${CMAKE_CURRENT_LIST_DIR}/PathfindingViewWidget3DPart.cpp
		${CMAKE_CURRENT_LIST_DIR}/AboutWindow.h
        ${CMAKE_CURRENT_LIST_DIR}/AdvancedOptionWindow.h
    	${CMAKE_CURRENT_LIST_DIR}/AtlasViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/BondViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/InputWindow.h
    	${CMAKE_CURRENT_LIST_DIR}/Mesh3D.h
    	${CMAKE_CURRENT_LIST_DIR}/NodeParentChildernViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/SharedDataGUI.h
    	${CMAKE_CURRENT_LIST_DIR}/SweepViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/CayleySpaceViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/CustomSlider.h
    	${CMAKE_CURRENT_LIST_DIR}/Camera.h
    	${CMAKE_CURRENT_LIST_DIR}/Renderer.h
    	${CMAKE_CURRENT_LIST_DIR}/CayleyPointRenderingWrapper.h
    	${CMAKE_CURRENT_LIST_DIR}/RealisationViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/MainWindow.h
    	${CMAKE_CURRENT_LIST_DIR}/RealizationViewWidgetCornerVersion.h
    	${CMAKE_CURRENT_LIST_DIR}/ConstraintSelectionWindow.h
    	${CMAKE_CURRENT_LIST_DIR}/ConstraintSelectionMoleculeViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/ConstraintSelectionBondViewWidget.h
    	${CMAKE_CURRENT_LIST_DIR}/MessageWindow.h
    	${CMAKE_CURRENT_LIST_DIR}/PathfindingViewWidget3DPart.h
    	${CMAKE_CURRENT_LIST_DIR}/PseudoAtlasWindow.h
    	${CMAKE_CURRENT_LIST_DIR}/StatsWindow.h
    	${CMAKE_CURRENT_LIST_DIR}/CayleyStatisticsWindow.h
    PUBLIC
)
