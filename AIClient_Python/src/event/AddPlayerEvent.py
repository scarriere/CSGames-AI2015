'''
Created on Dec 17, 2014

@author: samuel
'''
from event.OutgoingEvent import OutgoingEvent

class AddPlayerEvent(OutgoingEvent):
    teamName = ""
    characterNames = []

    def __init__(self, teamName, characterNames):
        self.teamName = teamName
        self.characterNames = characterNames
        
    def toString(self):
        message = "Game:AddPlayer:"
        message += self.teamName
        
        for characterName in self.characterNames:
            message += self.SEPARATOR + characterName
        
        return message