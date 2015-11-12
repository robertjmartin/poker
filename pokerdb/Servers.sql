CREATE TABLE [dbo].[Servers]
(
	[Id] UNIQUEIDENTIFIER NOT NULL PRIMARY KEY, 
    [PhysicalServerId] INT NOT NULL,
	[Port] INT NOT NULL, 
	[Key] BINARY(50) NOT NULL, 
    [ServerStatusId] INT NOT NULL
)
