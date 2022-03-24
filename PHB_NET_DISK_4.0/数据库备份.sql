-- MySQL dump 10.13  Distrib 8.0.28, for Linux (x86_64)
--
-- Host: localhost    Database: net_disk
-- ------------------------------------------------------
-- Server version	8.0.28-0ubuntu0.20.04.3

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `filelist`
--

DROP TABLE IF EXISTS `filelist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `filelist` (
  `precode` int DEFAULT NULL,
  `code` int NOT NULL AUTO_INCREMENT,
  `filename` varchar(50) DEFAULT NULL,
  `belongID` int DEFAULT NULL,
  `filetype` varchar(5) DEFAULT NULL,
  `md5sum` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB AUTO_INCREMENT=149 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `filelist`
--

LOCK TABLES `filelist` WRITE;
/*!40000 ALTER TABLE `filelist` DISABLE KEYS */;
INSERT INTO `filelist` VALUES (0,143,'file',35,'f','620f0b67a91f7f74151bc5be745b7110'),(0,144,'file1',40,'f',''),(0,145,'file1',35,'f','1490c1f0745db1fbac3a85269810a5c5'),(0,148,'test',35,'d',NULL);
/*!40000 ALTER TABLE `filelist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `filepool`
--

DROP TABLE IF EXISTS `filepool`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `filepool` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `filemd5` varchar(200) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `linknum` int NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=82 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `filepool`
--

LOCK TABLES `filepool` WRITE;
/*!40000 ALTER TABLE `filepool` DISABLE KEYS */;
INSERT INTO `filepool` VALUES (79,'620f0b67a91f7f74151bc5be745b7110',1),(80,'',1),(81,'1490c1f0745db1fbac3a85269810a5c5',1);
/*!40000 ALTER TABLE `filepool` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usercmd`
--

DROP TABLE IF EXISTS `usercmd`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `usercmd` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `username` varchar(20) DEFAULT NULL,
  `usercmd` varchar(30) DEFAULT NULL,
  `cmdpath` varchar(30) DEFAULT NULL,
  `cmdtime` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=284 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `usercmd`
--

LOCK TABLES `usercmd` WRITE;
/*!40000 ALTER TABLE `usercmd` DISABLE KEYS */;
INSERT INTO `usercmd` VALUES (204,'yf','ls','yf/','2022-03-23 23:27:02'),(205,'yf','mkdir file','yf/','2022-03-23 23:27:16'),(206,'yf','ls','yf/','2022-03-23 23:27:19'),(207,'yf','rmdir file','yf/','2022-03-23 23:27:35'),(208,'yf','ls','yf/','2022-03-23 23:27:36'),(209,'yf','ls','yf/','2022-03-23 23:28:42'),(210,'yf','ls','yf/','2022-03-23 23:29:02'),(211,'yf','puts file','yf/','2022-03-23 23:29:12'),(212,'yf','ls','yf/','2022-03-23 23:29:17'),(213,'yf','gets file','yf/','2022-03-23 23:29:34'),(214,'yf','quit','yf/','2022-03-23 23:29:47'),(215,'yf','ls','yf/','2022-03-23 23:30:21'),(216,'yf','remove file','yf/','2022-03-23 23:30:32'),(217,'yf','ls','yf/','2022-03-23 23:30:33'),(218,'yf','puts file','yf/','2022-03-23 23:31:02'),(219,'zhangsan','ls','zhangsan/','2022-03-24 00:00:48'),(220,'zhangsan','quit','zhangsan/','2022-03-24 00:01:21'),(221,'luke','ls','luke/','2022-03-24 00:08:35'),(222,'luke','cd ..','luke/','2022-03-24 00:08:44'),(223,'luke','quit','luke/','2022-03-24 00:08:52'),(224,'ll','ls','ll/','2022-03-24 00:17:48'),(225,'ll','quit','ll/','2022-03-24 00:17:51'),(226,'ll','quit','ll/','2022-03-24 00:18:10'),(227,'nn','puts file1','nn/','2022-03-24 00:21:32'),(228,'nn','ls','nn/','2022-03-24 00:21:35'),(229,'yf','ls','yf/','2022-03-24 17:15:05'),(230,'yf','puts file1','yf/','2022-03-24 17:15:15'),(231,'yf','ls','yf/','2022-03-24 17:16:14'),(232,'yf','gets file','yf/','2022-03-24 17:17:53'),(233,'yf','quit','yf/','2022-03-24 17:18:43'),(234,'yf','ls','yf/','2022-03-24 22:06:11'),(235,'yf','mkdir text','yf/','2022-03-24 22:06:40'),(236,'yf','ls','yf/','2022-03-24 22:06:42'),(237,'yf','cd text','yf/text//','2022-03-24 22:06:46'),(238,'yf','ls','yf/text//','2022-03-24 22:06:49'),(239,'yf','pwd','yf/text//','2022-03-24 22:07:01'),(240,'yf','cd /','yf/','2022-03-24 22:07:14'),(241,'yf','ls','yf/','2022-03-24 22:07:15'),(242,'yf','pwd','yf/','2022-03-24 22:07:17'),(243,'yf','ls','yf/','2022-03-24 22:07:36'),(244,'yf','ls','yf/','2022-03-24 22:07:58'),(245,'yf','cd text','yf/text//','2022-03-24 22:08:01'),(246,'yf','ls','yf/text//','2022-03-24 22:08:03'),(247,'yf','quit','yf/text//','2022-03-24 22:08:39'),(248,'ll','quit','ll/','2022-03-24 22:08:49'),(249,'yf','ls','yf/','2022-03-24 22:15:55'),(250,'yf','rmdir text','yf/','2022-03-24 22:16:02'),(251,'yf','ls','yf/','2022-03-24 22:16:07'),(252,'yf','cd text','yf/text//','2022-03-24 22:16:18'),(253,'yf','pwd','yf/text//','2022-03-24 22:16:23'),(254,'yf','quit','yf/text//','2022-03-24 22:16:32'),(255,'yf','ls','yf/','2022-03-24 22:19:41'),(256,'yf','rmdir text','yf/','2022-03-24 22:19:48'),(257,'yf','ls','yf/','2022-03-24 22:19:50'),(258,'yf','quit','yf/','2022-03-24 22:19:56'),(259,'ll','ls','ll/','2022-03-24 22:22:19'),(260,'ll','quit','ll/','2022-03-24 22:22:27'),(261,'yf','ls','yf/','2022-03-24 22:22:36'),(262,'yf','mkdir test','yf/','2022-03-24 22:22:59'),(263,'yf','ls','yf/','2022-03-24 22:23:01'),(264,'yf','cd test','yf/test//','2022-03-24 22:23:04'),(265,'yf','pwd','yf/test//','2022-03-24 22:23:08'),(266,'yf','cd ..','yf/test/','2022-03-24 22:23:10'),(267,'yf','ls','yf/test/','2022-03-24 22:23:12'),(268,'yf','pwd','yf/test/','2022-03-24 22:23:17'),(269,'yf','cd ..','yf/test/','2022-03-24 22:23:29'),(270,'yf','ls','yf/test/','2022-03-24 22:23:32'),(271,'yf','pwd','yf/test/','2022-03-24 22:23:35'),(272,'yf','rmdir test','yf/test/','2022-03-24 22:24:02'),(273,'yf','ls','yf/test/','2022-03-24 22:24:04'),(274,'yf','quit','yf/test/','2022-03-24 22:24:06'),(275,'yf','ls','yf/','2022-03-24 22:56:46'),(276,'yf','mkdir test','yf/','2022-03-24 22:56:52'),(277,'yf','ls','yf/','2022-03-24 22:56:53'),(278,'yf','cd test','yf/test/','2022-03-24 22:56:56'),(279,'yf','ls','yf/test/','2022-03-24 22:56:58'),(280,'yf','pwd','yf/test/','2022-03-24 22:57:01'),(281,'yf','cd ..','yf/','2022-03-24 22:57:03'),(282,'yf','pwd','yf/','2022-03-24 22:57:05'),(283,'yf','quit','yf/','2022-03-24 23:06:17');
/*!40000 ALTER TABLE `usercmd` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `userlist`
--

DROP TABLE IF EXISTS `userlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `userlist` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Username` varchar(20) DEFAULT NULL,
  `salt` varchar(15) DEFAULT NULL,
  `UserPasswd` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=41 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `userlist`
--

LOCK TABLES `userlist` WRITE;
/*!40000 ALTER TABLE `userlist` DISABLE KEYS */;
INSERT INTO `userlist` VALUES (35,'yf','$6$Rzu7RNSC','*0'),(36,'phb','$6$UO4bQS06','*0'),(37,'zhangsan','$6$RErR022L','*0'),(38,'luke','$6$Y5DZ9j21','*0'),(39,'ll','$6$55jxU32S','*0'),(40,'nn','$6$8t7XFeyr','*0');
/*!40000 ALTER TABLE `userlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `userlogin`
--

DROP TABLE IF EXISTS `userlogin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `userlogin` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Username` varchar(20) DEFAULT NULL,
  `loginTime` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `userlogin`
--

LOCK TABLES `userlogin` WRITE;
/*!40000 ALTER TABLE `userlogin` DISABLE KEYS */;
INSERT INTO `userlogin` VALUES (3,'yf','2022-03-23 23:22:06'),(4,'yf','2022-03-23 23:24:20'),(5,'yf','2022-03-23 23:26:59'),(6,'yf','2022-03-23 23:30:18'),(7,'phb','2022-03-23 23:44:10'),(8,'zhangsan','2022-03-24 00:00:45'),(9,'luke','2022-03-24 00:08:33'),(10,'ll','2022-03-24 00:17:47'),(11,'ll','2022-03-24 00:18:01'),(12,'nn','2022-03-24 00:21:22'),(13,'yf','2022-03-24 17:15:03'),(14,'yf','2022-03-24 17:16:12'),(15,'yf','2022-03-24 22:06:09'),(16,'ll','2022-03-24 22:08:46'),(17,'yf','2022-03-24 22:15:54'),(18,'yf','2022-03-24 22:19:40'),(19,'ll','2022-03-24 22:22:18'),(20,'yf','2022-03-24 22:22:35'),(21,'yf','2022-03-24 22:56:45');
/*!40000 ALTER TABLE `userlogin` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-03-24 23:25:24
