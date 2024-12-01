set(chessCloneSources main.cpp )
set(chessCloneHeaders
        ChessScene.cpp
        ChessScene.h
        ChessPiece.cpp
        ChessPiece.h
        Grid.cpp
        Grid.h
)

add_executable(chessClone ${chessCloneSources} ${chessCloneHeaders})
target_link_libraries(chessClone PRIVATE pontilus)
target_include_directories(chessClone PRIVATE "${CMAKE_SOURCE_DIR}/../../src/")
