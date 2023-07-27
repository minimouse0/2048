from src.pos import IntPos
from typing import Any,Callable, overload
from enum import Enum
from src.pos import *
class BlockedReason(Enum):
    """无法移动或传送的原因"""
    NotBlocked=0
    Wall=1
    Piece=2
class Piece:
    """棋子，必须由棋盘承载，不可刻意操作使其离开棋盘"""
    def __init__(self,grid,value:int=2) -> None:
        self._grid:Grid|None=grid
        """棋子所处的棋盘格子，必须通过move方法移动，不可直接更改坐标"""
        self._value:int=value
    def getPos(self)->IntPos:
        return self._grid._pos
    def getGrid(self):
        return self._grid
    def move(self,direction:Direction)->BlockedReason:
        """移动棋子的时候，承载棋子的格子也要处理，否则没有格子承载棋子，棋子对应的内存资源将被释放，棋子就会消失"""
        return self.tp(self.getPos().getNearby([(direction,1)]))
    def tp(self,target:IntPos)->BlockedReason:
        """传送棋子"""
        chessboard=self._grid.getChessboard()
        if target.x<0 or target.x>=chessboard.xsize or target.y<0 or target.y>=chessboard.ysize:
            return BlockedReason.Wall
        elif chessboard.getGrid(target).getPiece() != None:
            return BlockedReason.Piece
        elif chessboard.getGrid(target).getPiece() == None:
            #只有这一步是成功传送
            #传送的逻辑是把自己转交给新格子，然后把原棋子清除
            #将棋子转交给目标位置
            self._grid.getChessboard().getGrid(target).place(self)
            #把原位置的棋子清除
            self._grid.clean()
            self._grid=chessboard.getGrid(target)
            return BlockedReason.NotBlocked
    def push(self,direction:Direction):
        """移动棋子并将其合并或直到无法移动"""
        while self.move(direction)==BlockedReason.NotBlocked:
            pass
        if self.move(direction)==BlockedReason.Piece:
            if self.getPieceBeside(direction).getValue()==self.getValue():
                #数值相同，可以合并
                newPiece=self.getPieceBeside(direction).merge(self)
                #再尝试向下看看这次能不能合并
                newPiece.push(direction)
    def merge(self,targetPiece):
        #自己加上目标棋子数值，然后目标棋子所在格子清空
        self._value+=targetPiece.getValue()
        targetPiece.getGrid().clean()
        return self
    def abandon(self):
        self._grid=None
    def getValue(self)->int:
        return self._value
    def getPieceBeside(self,direction:Direction):
        return self._grid.getChessboard().getGrid(self.getPos().getNearby([(direction,1)])).getPiece()
class Grid:
    """棋盘格子"""
    def __init__(self,pos:IntPos,chessboard)->None:
        self._piece:Piece|None=None
        """为格子定义棋子容器，None表示空格子，后面所有可能访问到空格子的都要写None情况"""
        self._pos:IntPos=pos
        """这个pos是只读，因为棋盘格子是不可以改变位置的"""
        self._chessboard:Chessboard=chessboard
    def getPos(self)->IntPos:
        return self._pos
    def getPiece(self)->Piece|None:
        return self._piece
    def getChessboard(self):
        return self._chessboard
    @overload
    def place(self,value:int=2):
        pass
    @overload
    def place(self,value:Piece):
        pass
    def place(self,value:int|Piece=2):
        if self._piece!=None:
            pass
        elif type(value)==int:
            self._piece=Piece(self,value)
        elif type(value)==Piece:
            self._piece=value
        return self
    def clean(self)->Piece:
        """清空格子里的棋子，因为棋子没有被移动，所以会和持有它的实例解绑并直接从游戏里消失"""
        old_piece=self._piece
        #清除旧棋子的所有者
        old_piece.abandon()
        self._piece=None
        return old_piece
class Chessboard:
    """棋盘自身"""
    #横向为x，纵向为y
    def __init__(self,xsize:str,ysize:str)->None:
        self.xsize=xsize
        self.ysize=ysize
        self._grids:list[list[Grid]]=list()
        """外层为y，内层为x，访问的时候也是先确定纵向再确定横向"""
        #此处对最外层也就是纵向进行了遍历
        for y in range(0,ysize):
            row:list[Grid]=list()
            for x in range(0,xsize):
                row.append(Grid(IntPos(x,y),self))
            self._grids.append(row)
    def getGrid(self,pos:IntPos)->Grid:
        return self._grids[pos.y][pos.x]
    def forEach(self,function:Callable[[Grid],Any]):
        for y in range(0,self.ysize):
            for x in range(0,self.xsize):
                function(self.getGrid(IntPos(x,y)))
    def getRow(self,y:int)->list[Grid]:
        return self._grids[y]
    def getColumn(self,x:int)->list[Grid]:
        column=list()
        for y in range(0,self.ysize):
            column.append(self.getRow(y)[x])
        return column
    def getChessboard(self)->list[list[Grid]]:
        return self._grids
    def push(self,direction:Direction):
        if direction==Direction.Up:
            for column in self._grids:
                for grid in column:
                    if grid.getPiece()==None:
                        continue
                    grid.getPiece().push(Direction.Up)
        elif direction==Direction.Down:
            for column in reversed(self._grids):
                for grid in column:
                    if grid.getPiece()==None:
                        continue
                    grid.getPiece().push(Direction.Down)  
        elif direction==Direction.Left:
            for column in self._grids:
                for grid in column:
                    if grid.getPiece()==None:
                        continue
                    grid.getPiece().push(Direction.Left)        
        elif direction==Direction.Right:
            for column in self._grids:
                for grid in reversed(column):
                    if grid.getPiece()==None:
                        continue
                    grid.getPiece().push(Direction.Right)      