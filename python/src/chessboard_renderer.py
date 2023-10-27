from src.chessboard import *
import sys,time,asyncio
#import pygame
# 使用pygame之前必须初始化
#pygame.init()
chessboard_on_screen=None


'''
# 设置主屏窗口
screen = pygame.display.set_mode((400,400))
# 设置窗口的标题，即游戏名称
pygame.display.set_caption('hello world')

f=pygame.font.Font("/Users/huachengxu/Library/Fonts/consola.ttf",60)
text=f.render("2",True,(255,255,255),"brown")
#获得显示对象的rect区域坐标
textRect =text.get_rect()
# 设置显示对象居中
textRect.center = (200,200)

#创建一个 50*50 的图像,并优化显示
face = pygame.Surface((50,80),flags=pygame.HWSURFACE)
#填充颜色
face.fill(color='cyan')
screen.blit(face,(150,160))
'''
def fixed_length(arg:int,len:int)->str:
    temp=arg
    arglen=1
    while temp>=10:
        temp/=10
        arglen+=1
    res=""
    for i in range(0,len-arglen):
        res=res+" "
    res=res+str(arg)
    return res

def refresh_chessboard(chessboard:Chessboard):
    global chessboard_on_screen
    chessboard_on_screen=chessboard
    return render_chessboard(chessboard)

def render_chessboard(chessboard:Chessboard|None)->str|None:
    text=""
    if chessboard is None:
        return None
    #刷新屏幕为黑色
    #screen.fill('black')
    #计算命令行上显示的字符
    for row in chessboard.getChessboard():

        text=text+"---------------------\n"
        text=text+"|"

        for grid in row:
            text=text+render_grid(grid)+"|"
            #grid_surface=render_grid(grid)
            #screen.blit(grid_surface,grid_surface.get_rect())
        text=text+"\n"
    
    #pygame.display.update() #更新屏幕内容

    text=text+"y--------------------"
    return text

def render_grid(grid:Grid,length:int=4)->str|None:
    piece:Piece|None=grid.getPiece()
    text=str()
    
    if piece is None:#空格子
        for i in range(0,length):
            text=text+" "
        #return None
    else:
        text=text+fixed_length(piece.getValue(),length)
    #text=f.render(str(piece.getValue()),True,(255,255,255),"brown")
    #获得显示对象的rect区域坐标
    #textRect =text.get_rect()
    # 设置显示对象居中
    #textRect.center = (piece.getPos().x*100,piece.getPos().y*100)
    #text.get_rect().center=(piece.getPos().x*100,piece.getPos().y*100)
    #screen.blit(text,textRect)
    return text
async def displayLoop():
    while True:
        # 循环获取事件，监听事件状态
        for event in pygame.event.get():
            # 判断用户是否点了"X"关闭按钮,并执行if代码段
            if event.type == pygame.QUIT:
                #卸载所有模块
                pygame.quit()
                #终止程序，确保退出程序
        render_chessboard(chessboard_on_screen)
        yield asyncio.sleep(0.02)
loop=asyncio.get_event_loop()
task=[displayLoop()]
#loop.run_until_complete(asyncio.gather(*task))