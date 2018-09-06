CREATE DATABASE ContextualizationTool;

CREATE TABLE Language (
	ID varchar(10) NOT NULL,
	Name varchar(50) NOT NULL,
	TS TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	PRIMARY KEY (ID)
);

CREATE TABLE String (
	ID varchar(100) NOT NULL,
	Value varchar(255) NOT NULL,
	Description varchar(255),
	MaxLenght varchar(50),
	State varchar(50),
	LanguageID varchar(10),
	PRIMARY KEY (ID),
	FOREIGN KEY (LanguageID) REFERENCES Language(ID)
);

INSERT INTO Language (ID, Name) VALUES ('es', 'spanish');
INSERT INTO Language (ID, Name) VALUES ('en', 'english');
INSERT INTO Language (ID, Name) VALUES ('fr', 'french');
INSERT INTO Language (ID, Name) VALUES ('de', 'deutsch');
INSERT INTO Language (ID, Name) VALUES ('it', 'italian');
INSERT INTO Language (ID, Name) VALUES ('ja', 'japanese');