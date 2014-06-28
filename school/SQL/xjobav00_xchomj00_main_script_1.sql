--IDS 2012 skript {xchomj00,xjobav00}@stud.fit.vutbr.cz

ALTER SESSION SET NLS_DATE_FORMAT='yyyy/mm/dd hh24:mi';
--Vymazanie tabuliek

DROP TABLE Pracovnik CASCADE CONSTRAINTS;
DROP TABLE Vyrobok CASCADE CONSTRAINTS;
DROP TABLE Pocet CASCADE CONSTRAINTS;
DROP TABLE Zakaznik CASCADE CONSTRAINTS;
DROP TABLE Stretava CASCADE CONSTRAINTS;
DROP TABLE Objednavka CASCADE CONSTRAINTS;
DROP TABLE Faktura CASCADE CONSTRAINTS;
DROP TABLE Specializuje CASCADE CONSTRAINTS;
DROP SEQUENCE seq_ID_Pracovnik;
DROP SEQUENCE seq_ID_Zakaznik;
DROP SEQUENCE seq_ID_Vyrobok;
DROP SEQUENCE  seq_ID_Objednavka ;
DROP SEQUENCE seq_ID_Faktura;
--Vytvorenie tabuliek 
  
CREATE TABLE Pracovnik(
ID_Pracovnik INT NOT NULL,                  
Typ_Uvazku VARCHAR(20) NOT NULL,
Rodne_Cislo NUMERIC(10,0) NOT NULL,
Pozicia VARCHAR(70) NOT NULL,
Meno VARCHAR(40) NOT NULL,
Priezvisko VARCHAR(40) NOT NULL,
Adresa VARCHAR(200) NOT NULL,
Tel_c  VARCHAR(50) ,
Je_Veduci NUMERIC(1,0) DEFAULT 0 
);
 CREATE SEQUENCE seq_ID_Pracovnik;

--CREATE INDEX PRACOVNIK_INDEX ON Pracovnik(ID_Pracovnik);

CREATE INDEX "XCHOMJ00"."PRACOVNIK_INDEX" ON "XCHOMJ00"."PRACOVNIK"
  (
    "ID_PRACOVNIK"
  )
  PCTFREE 10 INITRANS 2 MAXTRANS 255 STORAGE
  (
    INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645 PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT
  )
  TABLESPACE "USERS" ;




CREATE TABLE Stretava(
ID_Zakaznik INT NOT NULL,
ID_Pracovnik INT,
Miesto VARCHAR(200) NOT NULL,
CAS_OD DATE NOT NULL,
CAS_DO DATE DEFAULT NULL
);


CREATE TABLE Zakaznik(
ID_Zakaznik INT NOT NULL ,
ID_Pracovnik INT NOT NULL,
Meno VARCHAR(40) NOT NULL,
Priezvisko VARCHAR(40) NOT NULL,
Bankove_spojenie VARCHAR(100),
Tel_c VARCHAR(50),
Email VARCHAR(80)
);
 CREATE SEQUENCE seq_ID_Zakaznik;
 

CREATE INDEX ZAKAZNIK_INDEX ON Zakaznik(ID_Zakaznik,ID_Pracovnik);


CREATE TABLE Vyrobok(
ID_Vyrobok INT NOT NULL,
Nazov VARCHAR(100) NOT NULL,
Cena NUMERIC(36,2) NOT NULL,
Typ VARCHAR(50)
);
 CREATE SEQUENCE seq_ID_Vyrobok;
 

CREATE INDEX VYROBOK_INDEX ON Vyrobok(ID_VYROBOK);



CREATE TABLE Pocet(
ID_Vyrobok INT NOT NULL,
ID_Objednavka INT NOT NULL,
Mnozstvo INT DEFAULT 1
);

CREATE TABLE Specializuje(
ID_Vyrobok INT NOT NULL,
ID_Pracovnik INT NOT NULL
);

CREATE TABLE Objednavka(
ID_Objednavka INT NOT NULL,
ID_Faktura INT NOT NULL,
ID_Zakaznik INT NOT NULL,
Datum_objednania DATE NOT NULL,
Datum_dodania DATE NOT NULL
);
CREATE SEQUENCE seq_ID_Objednavka;

CREATE TABLE Faktura(
ID_Faktura INT NOT NULL,
ID_Pracovnik INT NOT NULL,
Cena NUMERIC(36,2) NOT NULL,
Doba_Splatnosti DATE NOT NULL,
je_splatena NUMERIC(1,0) DEFAULT 0,
bankovy_ucet  VARCHAR(50) DEFAULT NULL, 
Sposob_platby VARCHAR(150)
);
CREATE SEQUENCE seq_ID_Faktura;


-- Nastavenie primarnych klucov  

ALTER TABLE Pracovnik ADD CONSTRAINT PK_Pracovnik PRIMARY KEY (ID_Pracovnik);
ALTER TABLE Zakaznik ADD CONSTRAINT PK_Zakaznik PRIMARY KEY (ID_Zakaznik);
ALTER TABLE Objednavka ADD CONSTRAINT PK_Objednavka PRIMARY KEY (ID_Objednavka);
ALTER TABLE Faktura ADD CONSTRAINT PK_Faktura PRIMARY KEY (ID_Faktura);
ALTER TABLE Vyrobok ADD CONSTRAINT PK_Vyrobok PRIMARY KEY (ID_Vyrobok);
ALTER TABLE Stretava ADD CONSTRAINT PK_Stretava PRIMARY KEY (ID_Pracovnik,ID_Zakaznik,CAS_OD); 
ALTER TABLE Pocet ADD CONSTRAINT PK_Pocet PRIMARY KEY (ID_Vyrobok,ID_Objednavka);  
ALTER TABLE Specializuje ADD CONSTRAINT PK_Specializuje PRIMARY KEY (ID_Vyrobok,ID_Pracovnik);

-- Nastavenie cudzich klucov 


ALTER TABLE Zakaznik ADD CONSTRAINT FK_Zakaznik_Pracovnik FOREIGN KEY (ID_Pracovnik) REFERENCES Pracovnik;
ALTER TABLE Faktura ADD CONSTRAINT FK_Pracovnik_Faktura FOREIGN KEY (ID_Pracovnik) REFERENCES Pracovnik;
ALTER TABLE Objednavka ADD CONSTRAINT FK_Faktura_Objednavka FOREIGN KEY (ID_Faktura) REFERENCES Faktura ON DELETE CASCADE;
ALTER TABLE Objednavka ADD CONSTRAINT FK_Objednavka_Zakaznik FOREIGN KEY (ID_Zakaznik) REFERENCES Zakaznik ON DELETE CASCADE;
ALTER TABLE Stretava ADD CONSTRAINT FK_Stretava_Pracovnik FOREIGN KEY (ID_Pracovnik) REFERENCES Pracovnik;
ALTER TABLE Stretava ADD CONSTRAINT FK_Stretava_Zakaznik FOREIGN KEY (ID_Zakaznik) REFERENCES Zakaznik;
ALTER TABLE Specializuje ADD CONSTRAINT FK_Pracovnik_Specializuje FOREIGN KEY (ID_Pracovnik) REFERENCES Pracovnik;
ALTER TABLE Specializuje ADD CONSTRAINT FK_Vyrobok_Specializuje FOREIGN KEY (ID_Vyrobok) REFERENCES Vyrobok; 
ALTER TABLE Pocet ADD CONSTRAINT FK_Vyrobok_Pocet FOREIGN KEY (ID_Vyrobok) REFERENCES Vyrobok ON DELETE CASCADE;
ALTER TABLE Pocet ADD CONSTRAINT FK_Objednavka_Pocet FOREIGN KEY (ID_Objednavka) REFERENCES Objednavka ON DELETE CASCADE; 


--- DB TRIGGER vypocet ceny faktury
create or replace
trigger calc_cost_faktura after insert or delete on Pocet
declare

currval_tmp integer;
actual_tmp integer;
local_cost faktura.cena%TYPE;
nextval_tmp integer;
begin
   select seq_ID_Faktura.nextval into nextval_tmp from dual;
   select seq_ID_Faktura.CURRVAL into currval_tmp from dual;
   actual_tmp:=0;
while actual_tmp < currval_tmp loop

select sum(v.cena*p.mnozstvo) into local_cost from vyrobok v join pocet p on v.id_vyrobok=p.id_vyrobok join objednavka o on p.id_objednavka = o.id_objednavka left join faktura f on f.id_faktura=o.id_faktura
 where f.id_faktura=actual_tmp;
 
 
 
 if local_cost is not NULL then 
  update faktura set cena = local_cost
   where ID_Faktura=actual_tmp;
else
 update faktura set cena = 0
 where ID_faktura=actual_tmp; 
end if; 
  actual_tmp:=actual_tmp+1; 
end loop;
end;
/
------------------------------------


---Pridaj vstupne data do kazdej tabulky do kazdej    !!!!
INSERT INTO Pracovnik(ID_Pracovnik,Typ_Uvazku,Rodne_Cislo,Pozicia,Meno,Priezvisko,Adresa,Tel_c,Je_Veduci) VALUES(seq_ID_Pracovnik.nextVal,'trvaly',1234567987,'majitel','Jaroslav','Filip','Nebeska 85, Bratislava','+420 897546213',1);

COMMIT;
 
