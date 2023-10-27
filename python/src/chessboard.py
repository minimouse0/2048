import copy
from src.pos import IntPos
from typing import Any,Callable, overload
from enum import Enum
from src.pos import *
import random
class BlockedReason(Enum):
    """无法移动或传送的原因"""
    NotBlocked=0
    """没有出现无法移动的情况"""
    Wall=1
    """撞墙"""
    Piece=2
    """被棋子挡住"""
class Piece:
    """棋子，必须由棋盘承载，不可刻意操作使其离开棋盘"""
    def __init__(self,grid,value:int=2) -> None:
        self._grid:Grid|None=grid
        """棋子所处的棋盘格子，必须通过move方法移动，不可直接更改坐标"""
        self._value:int=value
        self._merged=False
    def markMerged(self):
        self._merged=True
    def markUnmerged(self):
        self._merged=False
    def isMarkedMerged(self)->bool:
        return self._merged
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
        """移动棋子不合并"""
        while self.move(direction)==BlockedReason.NotBlocked:#将棋子移动直到被挡住
            pass

    def merge(self,targetPiece):
        """自己加上目标棋子数值，然后目标棋子所在格子清空"""
        self._value+=targetPiece.getValue()
        targetPiece.getGrid().clean()
        return self
    def pushandmerge(self,direction:Direction):
        """
        移动棋子并将其合并或直到无法移动
        """
        self.push(direction)#先移动棋子直到被挡住
        #print(self.isMarkedMerged())
        if self.move(direction) is BlockedReason.Piece:#如果棋子被棋子挡住，判断是否能合并，并且同时棋子没被合并过
            beside=self.getPieceBeside(direction)
            if beside.getValue() is self.getValue() and not self.isMarkedMerged() and not beside.isMarkedMerged():
                #数值相同，可以合并
                newPiece=beside.merge(self).markMerged()
                #如果合并成功，往回找棋子并
                #再尝试向下看看这次能不能合并
                #newPiece.push(direction)
            #print(self.getValue())#
    def abandon(self):
        self._grid=None
    def getValue(self)->int:
        """棋子的值"""
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
    def getAllEmptyGrids(self)->list[Grid]:
        result=list()
        def currentGridHandler(currentGrid:Grid):
            if currentGrid.getPiece() is None:
                result.append(currentGrid)
        self.forEach(currentGridHandler)
        return result
    def getAllPieces(self)->list[Piece]:        
        result=list()
        def currentGridHandler(currentGrid:Grid):
            if currentGrid.getPiece() is not None:
                result.append(currentGrid.getPiece())
        self.forEach(currentGridHandler)
        return result
    def clearMergedMarks(self):
        for currentPiece in self.getAllPieces():
            currentPiece.markUnmerged()
    def chessboardComparison(self,chessboard)->bool:
        """
        比较自己和传入的另一个棋盘是否相同
        :returns 棋盘是否完全相同
        """
        self.ComparisonMatch=True
        """这个match是用来让下面currentGridHandler那个回调函数能共享到"""
        chessboardcopy=copy.deepcopy(chessboard)
        def currentGridHandler(currentGrid:Grid):
            currentGridPos=currentGrid.getPos()
            if currentGrid.getPiece() is None:#第一种情况，格子为空，无法比较数值，直接判断是否也为空
                if self.getGrid(currentGridPos).getPiece() is not None:
                    self.ComparisonMatch=False
            else:#第二种情况，格子里有东西，判断自己这里有东西还是没有东西，如果有，数值是否相等
                if self.getGrid(currentGridPos).getPiece() is None:
                    self.ComparisonMatch=False
                elif currentGrid.getPiece().getValue() is not self.getGrid(currentGridPos).getPiece().getValue():
                    self.ComparisonMatch=False
        chessboardcopy.forEach(currentGridHandler)
        return self.ComparisonMatch
    def push(self,direction:Direction):
        """移动棋盘"""
        '''
        所有的棋子被移动后都会被临时标记一次已合并
        一次移动后，函数会尝试再移动一次
        第二次移动时，所有已经合并完的棋子都不会参与任何合并
        全部流程结束后，所有棋子的已合并标记都会被移除
        移动之前，程序会创建一个棋盘的副本，然后先移动这个新横盘
        如果这个新横盘按照方向移动之后内容和原棋盘没有区别
        就这步移动就会失败
        如果移动失败，首先，新棋子都不会生成
        如果四个方向都移动失败，那就是死局
        '''
        if direction==Direction.Up:
            for column in self._grids:
                for grid in column:#两层for用来遍历所有格子
                    if grid.getPiece()==None:#这个格子是空的，不需要移动
                        continue
                    grid.getPiece().pushandmerge(Direction.Up)
        elif direction==Direction.Down:
            for column in reversed(self._grids):
                for grid in column:
                    if grid.getPiece()==None:
                        continue
                    grid.getPiece().pushandmerge(Direction.Down)  
        elif direction==Direction.Left:
            for column in self._grids:
                for grid in column:
                    if grid.getPiece()==None:
                        continue
                    grid.getPiece().pushandmerge(Direction.Left)        
        elif direction==Direction.Right:
            for column in self._grids:
                for grid in reversed(column):
                    if grid.getPiece()==None:
                        continue
                    grid.getPiece().pushandmerge(Direction.Right)
        self.clearMergedMarks()
    def canPush(self,direction:Direction)->bool:
        """
        将现有棋盘深拷贝，然后移动这个棋盘，如果移动后和原横盘完全相同，则证明无法移动
        """
        chessboardcopy=copy.deepcopy(self) 
        chessboardcopy.push(direction)
        return not chessboardcopy.chessboardComparison(self)
    def tryPush(self,direction:Direction)->bool:
        canPush=self.canPush(direction)
        if canPush is True:
            self.push(direction)
        return canPush
    def playerPush(self,direction:Direction)->bool:
        """
        由玩家移动棋盘，包含尝试移动和生成新棋子
        """
        pushSuccess=self.tryPush(direction)
        if pushSuccess is True:
            self.randomNew()
        else:
            print("玩家移动：无法移动！")
        return pushSuccess

    def randomNew(self):
        randomarg=random.randrange(0,self.xsize*self.ysize)
        piecenum=2
        if randomarg<=1:
            piecenum=4
        emptyGrids=self.getAllEmptyGrids()
        emptyGrids[randomarg%len(emptyGrids)].place(piecenum)
    def checkGameOver(self)->bool:
        availale=False
        for direction in Direction:
            if self.canPush(direction) is True:
                availale=True
        return not availale