/* 20.05.2020
 *  LUMIERES DANS LA NUIT
 *  
 *  Petite simulation d'un emetteur morse.
 *  
 *  L'appui sur un interrupteur simule le manipulateur
 *  Une diode simule le message
 *  Le message envoyé est affiché sur un ecran (ici moniteur serie)
 *  
 *  ***
 *  
 *  le duree d'un point (di) est nommee dt
 *  
 *  si l'interrupteur est pressé pendant une duree inferieur a dt
 *  => c'est un point
 *  
 *  si 'l'interrupteur est pressé pendant une duree superieure a 3*dt
 *  => c'est un trait
 *  
 *  si l'interrupteur est relaché pendant n*dt
 *  => n espaces
 *  
 *  si l'interrupteur est pressé pendant une duree comprise entre dt et 3*dt
 *  => ?, signe incompris
 */

// #### PARAMETRES DE MONTAGES
// a adapter au montage réel

const byte m_pinLed = 13; // branchement de la led
const byte m_pinSwitch = 2; // branchement de l'interrupteur

// #### PARAMETRES D'UTILISATIONS
// peuvent être ajustés manuellement

const float m_dt = 1;
  // duree correspondant a un point (en seconde)
  // un trait => 3 dt

// #### PARAMETRES INTERNES
// ne pas modifier

unsigned long m_pressedAt;
  // data a laquelle l'interrupteur a été pressé / relaché
  
unsigned long m_pressedFor;
  // duree pendant laquelle est pressee l'interrupteur

bool m_switchState = false;
  // etat initial de l'interrupteur


void setup() {
  // initialisation des pins
  pinMode(m_pinLed, OUTPUT);
  digitalWrite(m_pinLed, LOW);
  
  pinMode(m_pinSwitch, INPUT);

  // initialisation du port serie
  Serial.begin(9600);
  
  // initialisation des parametres
  m_switchState = digitalRead(m_pinSwitch);
  m_pressedAt = millis();

}

void loop() {

  bool newState = digitalRead(m_pinSwitch);

  if (newState != m_switchState){ // si l'etat de l'interrupteur a change
    
    // calcul de la duree durant laquelle l'interrupteur est resté a l'état précedent
    m_pressedFor = millis() - m_pressedAt; 
    
    // enregistrement de la date du dernier changement d'etat de l'interrupteur a changé d'état
    m_pressedAt = millis();

    // changement d'etat de la led
    digitalWrite(m_pinLed, newState);

    if ((m_pressedFor < m_dt*1000) && m_switchState) { // duree d'un point
      Serial.print('.');  
    } else if ((m_pressedFor > 3*m_dt*1000) && m_switchState){ // c'est un trait
      Serial.print('_');
    } else if (!m_switchState) {
      // ecrit le nombre d'espace correpondant a la duree ou l'interrupteur est relaché
      int spaceNbr = m_pressedFor / (m_dt* 1000); 
      for (int k = 0; k < spaceNbr ; k++){
        Serial.print(' ');
      }
    } else { // duree d'appui non reconnu
      Serial.print('?');
    }
  }

  m_switchState = newState;
}
