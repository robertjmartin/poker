﻿CREATE TABLE [dbo].[Seats]
(
	[Id] INT NOT NULL PRIMARY KEY, 
    [TableId] UNIQUEIDENTIFIER NOT NULL, 
    [Key] BINARY(50) NULL, 
    [ChipCount] INT NULL
)
