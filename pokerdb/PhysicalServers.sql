CREATE TABLE [dbo].[PhysicalServers]
(
	[PhysicalServerId] INT NOT NULL PRIMARY KEY, 
    [Address] NVARCHAR(50) NOT NULL, 
    [Available] BIT NOT NULL
)
