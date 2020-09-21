-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb2+deb7u1
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tiempo de generación: 06-09-2014 a las 09:19:10
-- Versión del servidor: 5.5.38
-- Versión de PHP: 5.4.4-14+deb7u12

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de datos: `cm_data`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `config`
--

CREATE TABLE IF NOT EXISTS `config` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `parametro` varchar(64) NOT NULL,
  `valor` varchar(64) NOT NULL,
  `descr` varchar(128) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Volcado de datos para la tabla `config`
--

INSERT INTO `config` (`id`, `parametro`, `valor`, `descr`) VALUES
(1, 'log_sys', '/var/log/cm_data_sys.log', 'Archivo de logs de eventos de sistema'),
(2, 'log_modem', '/var/log/cm_data_modem.log', 'Archivo de logs de eventos de Cablemodems'),
(3, 'log_error', '/var/log/cm_data_error.log', 'Archivo de logs de errores'),
(4, 'procesos', '8', 'Cantidad de procesos hijo para consultar modems');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `equipos`
--

CREATE TABLE IF NOT EXISTS `equipos` (
  `cmts_id` int(3) NOT NULL AUTO_INCREMENT,
  `desc` varchar(128) NOT NULL,
  `ip_add` varchar(16) NOT NULL,
  `comunidad` varchar(64) NOT NULL,
  PRIMARY KEY (`cmts_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

--
-- Volcado de datos para la tabla `equipos`
--

INSERT INTO `equipos` (`cmts_id`, `desc`, `ip_add`, `comunidad`) VALUES
(1, 'ARRIS C4c', '192.168.100.10', 'docsiscomm'),
(2, 'ARRIS C3', '172.16.16.245', 'docsiscomm');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `oids`
--

CREATE TABLE IF NOT EXISTS `oids` (
  `id` int(3) NOT NULL AUTO_INCREMENT,
  `oid_str` varchar(256) NOT NULL,
  `oid_desc` varchar(64) NOT NULL,
  `tipo` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=21 ;

--
-- Volcado de datos para la tabla `oids`
--

INSERT INTO `oids` (`id`, `oid_str`, `oid_desc`, `tipo`) VALUES
(1, 'SNMPv2-SMI::transmission.127.1.3.3.1.2', 'CM_MAC', 'CMTS'),
(2, 'SNMPv2-SMI::transmission.127.1.3.3.1.3', 'CM_IP', 'CMTS'),
(3, 'SNMPv2-SMI::transmission.127.1.3.3.1.13', 'CM_US_SNR', 'CMTS'),
(4, 'SNMPv2-SMI::transmission.127.1.3.3.1.6', 'US_PWR_CMTS', 'CMTS'),
(5, 'SNMPv2-SMI::transmission.127.1.1.1.1.6.3', 'DS1_PWR', 'CM'),
(6, 'SNMPv2-SMI::transmission.127.1.1.1.1.6.48', 'DS2_PWR', 'CM'),
(7, 'SNMPv2-SMI::transmission.127.1.1.1.1.6.49', 'DS3_PWR', 'CM'),
(8, 'SNMPv2-SMI::transmission.127.1.1.1.1.6.50', 'DS4_PWR', 'CM'),
(9, 'SNMPv2-SMI::transmission.127.1.1.4.1.5.3', 'DS1_SNR', 'CM'),
(10, 'SNMPv2-SMI::transmission.127.1.1.4.1.5.48', 'DS2_SNR', 'CM'),
(11, 'SNMPv2-SMI::transmission.127.1.1.4.1.5.49', 'DS3_SNR', 'CM'),
(12, 'SNMPv2-SMI::transmission.127.1.1.4.1.5.50', 'DS4_SNR', 'CM'),
(13, 'SNMPv2-SMI::transmission.127.1.2.2.1.3.2', 'US1_PWR', 'CM'),
(14, 'SNMPv2-SMI::transmission.127.1.1.4.1.6.3', 'MIC', 'CM'),
(15, 'IF-MIB::ifInOctets.3', 'DS1_TF', 'CM'),
(16, 'IF-MIB::ifInOctets.48', 'DS2_TF', 'CM'),
(17, 'IF-MIB::ifInOctets.49', 'DS3_TF', 'CM'),
(18, 'IF-MIB::ifInOctets.50', 'DS4_TF', 'CM'),
(19, 'IF-MIB::ifOutOctets.4', 'US1_TF', 'CM'),
(20, 'IF-MIB::ifOutOctets.80', 'US2_TF', 'CM');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
