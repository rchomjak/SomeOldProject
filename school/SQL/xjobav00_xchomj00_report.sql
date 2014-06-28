select f.ID_Faktura,f.ID_Pracovnik,p.priezvisko AS Priezvisko_pracovnika ,f.cena,f.bankovy_ucet,f.sposob_platby,o.ID_Objednavka, 
o.Datum_objednania,o.Datum_dodania,
o.ID_Zakaznik,f.doba_splatnosti  from faktura f join objednavka o on f.ID_Faktura=o.ID_Faktura join pracovnik p on p.ID_Pracovnik=f.ID_Pracovnik where f.ID_Faktura=:param_name;