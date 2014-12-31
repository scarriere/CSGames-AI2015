'''
Created on Dec 21, 2014

@author: samuel
'''

from enum import Enum
from event.MoveCharacterEvent import MoveCharacterEvent
from aiclient.Singleton import Singleton
from event.QueueController import QueueController
from world.World import World
from event.DropMineEvent import DropMineEvent

class AIStatus(Enum):
    INIT, LOWER_RIGHT, LOWER_LEFT, UPPER_RIGHT, UPPER_LEFT = range(5)

class AI(object):
    aiStatus = AIStatus.INIT
    queueController = Singleton(QueueController)
    world = Singleton(World)
    
    def tick(self):
        if self.aiStatus == AIStatus.INIT:
            self.init()
        elif self.aiStatus == AIStatus.LOWER_RIGHT:
            self.lowerRight()
        elif self.aiStatus == AIStatus.LOWER_LEFT:
            self.lowerLeft()
        elif self.aiStatus == AIStatus.UPPER_RIGHT:
            self.upperRight()
        elif self.aiStatus == AIStatus.UPPER_LEFT:
            self.upperLeft()
    
    def init(self):
        event1 = MoveCharacterEvent(0, 7, 0)
        self.queueController.outEvents.put(event1)
        event2 = MoveCharacterEvent(1, 0, 7)
        self.queueController.outEvents.put(event2)
        self.aiStatus = AIStatus.LOWER_RIGHT
    
    def lowerRight(self):
        character1 = self.world.getTeam(self.world.yourId).characters[0]
        character2 = self.world.getTeam(self.world.yourId).characters[1]
        if character1.positionX == 7 and character1.positionY == 0 and character2.positionX == 0 and character2.positionY == 7:
            event1 = MoveCharacterEvent(0, 0, 0)
            self.queueController.outEvents.put(event1)
            event2 = MoveCharacterEvent(1, 7, 7)
            self.queueController.outEvents.put(event2)
            self.aiStatus = AIStatus.LOWER_LEFT
    
    def lowerLeft(self):
        character1 = self.world.getTeam(self.world.yourId).characters[0]
        character2 = self.world.getTeam(self.world.yourId).characters[1]
        if character1.positionX == 0 and character1.positionY == 0 and character2.positionX == 7 and character2.positionY == 7:
            event1 = MoveCharacterEvent(0, 7, 7)
            self.queueController.outEvents.put(event1)
            event2 = MoveCharacterEvent(1, 0, 0)
            self.queueController.outEvents.put(event2)
            self.aiStatus = AIStatus.UPPER_RIGHT
            
            if character1.mineReady:
                event3 = DropMineEvent(character1.characterId)
                self.queueController.outEvents.put(event3)
    
    def upperRight(self):
        character1 = self.world.getTeam(self.world.yourId).characters[0]
        character2 = self.world.getTeam(self.world.yourId).characters[1]
        if character1.positionX == 7 and character1.positionY == 7 and character2.positionX == 0 and character2.positionY == 0:
            event1 = MoveCharacterEvent(0, 0, 7)
            self.queueController.outEvents.put(event1)
            event2 = MoveCharacterEvent(1, 7, 0)
            self.queueController.outEvents.put(event2)
            self.aiStatus = AIStatus.UPPER_LEFT
    
    def upperLeft(self):
        character1 = self.world.getTeam(self.world.yourId).characters[0]
        character2 = self.world.getTeam(self.world.yourId).characters[1]
        if character1.positionX == 0 and character1.positionY == 7 and character2.positionX == 7 and character2.positionY == 0:
            event1 = MoveCharacterEvent(0, 7, 0)
            self.queueController.outEvents.put(event1)
            event2 = MoveCharacterEvent(1, 0, 7)
            self.queueController.outEvents.put(event2)
            self.aiStatus = AIStatus.LOWER_RIGHT