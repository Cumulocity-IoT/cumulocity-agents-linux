/*
 * nmtslave.c - userdefined CANopen NMT functions
 *
 * Copyright (c) 2012 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header: /z2/cvsroot/library/co_lib/examples/m1_x86_socketcan/nmtslave.c,v 1.1 2012/07/27 08:51:58 oe Exp $
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * (not supported with examples)
 *
 *
 *
 *
 *------------------------------------------------------------------
 */


/*
*  \file nmtslave.c
*  \author port GmbH Halle (Saale)
*  $Revision: 1.1 $
*  $Date: 2012/07/27 08:51:58 $
*
*++ This file contains function templates for a CANopen device.
*++ The functions have influence to the state machine behaviour of the
*++ CANopen node.
*++ The user is responsible for the contents of the functions.
*++ Before using this template you must make a copy of them.
*-- Diese Datei beinhaltet Funktionen f�r ein CANopen Ger�t.
*-- Die Funktionen haben Einflu� auf das Verhalten der State Machine
*-- des CANopen Knotens.
*-- Der Anwender ist f�r den Inhalt der Funktionen verantwortlich.
*/

#include <stdio.h>

#include <cal_conf.h>

#include <co_drv.h>
#include <co_usr.h>
#include <co_nmt.h>

#include <examples.h>

/**
*
*++ \brief resetApplInd - reset the application
*-- \brief resetApplInd - setzt die Applikation zur�ck
*
*++ This function will reset the device's application.
*++ All application parameters from the Object Dictionary are set
*++ to the default values before this function is called.
*++ The user has to ensure that his application will be reset.
*++ Additionally, it is possible application to load parameters
*++ from a non volatile memory.
*-- Diese Funktion setzt die Ger�teapplikation zur�ck.
*-- Die Applikationsvariablen aus dem Objektverzeichnis werden
*-- vor diesem Funktionsaufruf auf ihre Standardwerte gesetzt.
*-- Der Nutzer ist f�r das R�cksetzen seiner Applikation verantwortlich.
*-- Zus�tzlich k�nnen hier Applikationsparameter aus einem nicht fl�chtigen
*-- Speicher geladen werden.
*
* \returns
* nothing
*
*/

void resetApplInd(void)
{
}

/**
*
*++ \brief resetCommInd - reset communication parameters
*-- \brief resetCommInd - setzt die Kommunikationsparameter zur�ck
*
*++ This function is user defined. 
*++ All communication parameters from the Object Dictionary besides the
*++ bit rate and the node ID are set 
*++ to the default values before this function is called.
*++ The default values are taken from the object dictionary's
*++ "default" value, stored as \b defaultVal in the 
*++ object dictionary's \e VALUE_DESC_T struct.
*++ All node-id depending things are reset to
*++ predefined connection set, taking care of the
*++ internal global Variable \e coNodeId,
*++ which is normally set by the library with the call \e initCanopen()
*++ which calls \e getNodeId().
*++ This is only done for the first 4 TPDs and RPDOs, all
*++ others are set to invalid.
*++
*++ In this function the CANopen node can get a new bit rate 
*++ from a DIP-Switch or non volatile memory.
*++ For the CAN controller
*++ bit rate initialisation, the user is responsible.
*++ Further communication parameters can be loaded from a non volatile
*++ memory too. 
*++ In this case the object dictionary values have to be overwritten
*++ using \e putObj() and \e setCommPar() .
*
*-- Diese Funktion wird vom Anwender definiert. Bevor diese Funktion
*-- aufgerufen wird, werden alle Kommunikationsparameter bis auf die
*-- Node ID und die Bitrate auf ihre Standardwerte zur�ckgesetzt..
*-- F�r die von der Node-ID abh�ngigen Werte , z.B PDO COB-IDs,
*-- wird die aktuelle Knotennummer ber�cksichtigt.
*-- diese ist in der globalen Variablen \e coNodeId gespeichert,
*-- welche mittels der Funktion \e getNodeId() zuvor ermittelt wurde.
*-- Dem Knoten kann eine neue Bitrate durch Lesen eines
*-- DIP-Switches oder eines nicht-fl�chtigen Speichers zugewiesen werden.
*-- Der Anwender ist f�r das Setzen der Bitrate auf dem CAN-Controller
*-- verantwortlich.
*-- Weiterhin ist es m�glich weitere Kommunikationsparameter aus einem
*-- nicht-fl�chtigen Speicher zu laden.
*
* \returns
* nothing
*
*/

void resetCommInd(void)
{
#ifdef NO_PRINTF_S
    PUTCHAR('R');
#else
    PRINTF("Communication reset\n");
    /* printf("bitRate %d, ",(int)bitRate); */
    PRINTF("node_id %d\n",(int)getNodeId());
#endif
    /* this is the point to set the CAN Controller to a new bitrate */
    /*
    Stop_CAN();
    Set_Baudrate(bitRate, NULL);
    Start_CAN();
    */
}


/*******************************************************************/
/**
*
*++ \brief newStateInd -  indicates transition to a new communcation state
*-- \brief newStateInd -  zeigt einen Zustands�bergang in der Kommunikation an
*
*++ This function will be indicated at the slave, if it will be forced 
*++ to an other communication state.
*++ It is called before the transition change.
*++ This user interface ensures a save behaviour of the application,
*++ before the node goes to an other state.
*-- Diese Funktion wird beim Slave vor einem Zustands�bergang in der 
*-- Kommunikationszustandsmaschine aufgerufen. In dieser Funktion kann
*-- der Anwender in seiner Applikation Sicherheitsvorkehrungen treffen.
*
* \retval CO_TRUE
* ok
* \retval CO_FALSE
*++ don't change the state to OPERATIONAL
*-- Node soll nicht nach OPERATIONAL gehen
*/

BOOL_T newStateInd(
     NODE_STATE_T  newState /* new state */
     )
{
    /* state classifiaction */
    
    switch (newState) {
        /* Standard CANopen states */
        case PRE_OPERATIONAL:
	    break;
        case OPERATIONAL:
            break;
        case STOPPED:
            break;
        default:
            break;
    }
    return(CO_TRUE);
    
}
/*______________________________________________________________________EOF_*/
