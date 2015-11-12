CREATE TABLE [dbo].[user]
(
	[Id] INT NOT NULL PRIMARY KEY, 
    [ChipBank] BIGINT NOT NULL, 
    [Password] BINARY(50) NOT NULL
)
