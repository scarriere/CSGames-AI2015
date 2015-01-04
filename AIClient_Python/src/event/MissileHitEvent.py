'''
Created on Jan 3, 2015

@author: samuel
'''
from event.IngoingEvent import IngoingEvent
from aiclient.Singleton import Singleton
from world.World import World

class MissileHitEvent(IngoingEvent):

    def __init__(self):
        self.hitEntity = 0
        self.hitTeamId = 0
        self.hitCharacterId = 0
        self.originTeamId = 0
        self.originCharacterId = 0
    
    def fillArguments(self, string):
        arguments = string.split(":", 5)
        
        self.hitEntity = int(arguments[0])
        self.hitTeamId = int(arguments[1])
        self.hitCharacterId = int(arguments[2])
        self.originTeamId = int(arguments[3])
        self.originCharacterId = int(arguments[4])
        
    def execute(self):
        world = Singleton(World)
        world.getTeam(self.originTeamId).characters[self.originCharacterId].missileHit()
        if(self.hitEntity == 1):
            world.getTeam(self.hitTeamId).characters[self.hitCharacterId].hitByMissile()
        elif(self.hitEntity == 2):
            world.getTeam(self.hitTeamId).characters[self.hitCharacterId].mineHit()
        elif(self.hitEntity == 3):
            world.getTeam(self.hitTeamId).characters[self.hitCharacterId].missileHit()        