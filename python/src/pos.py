from enum import Enum
class Direction(Enum):
    """枚举棋盘方向"""
    NoDirection=0
    Up=1
    Down=2
    Left=3
    Right=4
class IntPos:
    """与LLSE完全相同的整数坐标类"""
    def __init__(self,x:int,y:int,z:int=0,dimid:int=0) -> None:
        self.x:int=x
        self.y:int=y
        self.z:int=z
        self.dimid:int=dimid
        self.dim:str="主世界"
    def move(self,path:list[tuple[Direction,int]]):
        for action in path:
            if action[0]==Direction.Up:
                self.y-=action[1]
            elif action[0]==Direction.Down:
                self.y+=action[1]
            elif action[0]==Direction.Left:
                self.x-=action[1]
            elif action[0]==Direction.Right:
                self.x+=action[1]
        return self
    def getNearby(self,path:list[tuple[Direction,int]]):
        targetPos=IntPos(self.x,self.y,self.z,self.dimid)
        return targetPos.move(path)
