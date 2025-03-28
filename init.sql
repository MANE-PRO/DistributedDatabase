-- Create the first database `record1`
CREATE DATABASE IF NOT EXISTS record1;

-- Create the second database `record2`
CREATE DATABASE IF NOT EXISTS record2;



-- Flush privileges to apply the changes
FLUSH PRIVILEGES;

-- Switch to `record1` database
USE record1;

-- Create a table in `record1` database
CREATE TABLE IF NOT EXISTS record (
    id VARCHAR(255) PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

-- Switch to `record2` database
USE record2;

-- Create a table in `record2` database
CREATE TABLE IF NOT EXISTS record (
    id VARCHAR(255) PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

