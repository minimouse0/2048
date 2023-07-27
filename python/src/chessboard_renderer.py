from src.chessboard import *
def render_chessboard(chessboard:Chessboard)->str:
    text=str()
    for row in chessboard.getChessboard():
        text=text+"---------------------\n"
        text=text+"|"
        for grid in row:
            piece=grid.getPiece()
            text=text+render_grid(grid)+"|"
        text=text+"\n"
    text=text+"y--------------------"
    return text
def render_grid(grid:Grid,length:int=4)->str:
    piece:Piece|None=grid.getPiece()
    if piece==None:
        text=str()
        for i in range(0,length):
            text=text+" "
        return text
    return str(piece.getValue()).ljust(length)