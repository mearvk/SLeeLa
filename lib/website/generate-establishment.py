#!/usr/bin/env python3
"""Generate an ESTABLISHMENT.md in each airline folder under lib/website/.

Each airline subdirectory (the ones created by generate-airlines.py) gets an
ESTABLISHMENT.md capturing the company's establishment facts: legal name,
founder(s), founding year(s), headquarters, office/hub locations, contact
information, workforce size, and an average-employee-age note.

Sourcing / honesty note:
  Founding years, founders, headquarters, hubs, alliances, and parent entities
  are well-established public facts. "Average age of employees" is generally NOT
  disclosed per company, so it is given as a labelled industry-context estimate
  rather than a precise claim. Employee counts are approximate and move over
  time; they are labelled "approx." Treat the files as a maintained summary, not
  a legal or investor-grade record.

Usage:
  python3 lib/website/generate-establishment.py            # write all files
  python3 lib/website/generate-establishment.py --count    # list only
"""
import argparse
import sys
from pathlib import Path

# slug -> establishment record.
# Keys: name (legal/common), founders, founded, hq, offices (hubs/major
# offices), website, contact (general line), employees (approx), avg_age
# (estimate note), parent, alliance, iata, icao, notes.
AIRLINES = {
    "piedmont-airlines": dict(
        name="Piedmont Airlines, Inc.",
        founders="Richard A. Henson (as Henson Aviation)",
        founded="1961 (as Henson Aviation; scheduled service 1962)",
        hq="Salisbury Regional Airport, Wicomico County, Maryland, USA",
        offices="Salisbury (HQ); crew/maintenance bases incl. Charlotte, Philadelphia",
        website="https://piedmont-airlines.com",
        contact="Via https://piedmont-airlines.com (careers & corporate)",
        employees="~10,000 (approx.)",
        parent="American Airlines Group (wholly owned; operates as American Eagle)",
        alliance="oneworld (via American Airlines)",
        iata="PT", icao="PDT",
        notes="Regional carrier feeding the American Eagle network."),
    "delta-air-lines": dict(
        name="Delta Air Lines, Inc.",
        founders="Collett E. Woolman (and partners; from Huff Daland Dusters)",
        founded="1925 (as Huff Daland Dusters); Delta Air Service 1928",
        hq="Atlanta, Georgia, USA (1030 Delta Blvd)",
        offices="Hubs: Atlanta, Minneapolis/St. Paul, Detroit, Salt Lake City, New York (JFK/LGA), Boston, Los Angeles, Seattle",
        website="https://www.delta.com",
        contact="+1-800-221-1212 (US reservations); https://www.delta.com/contactus",
        employees="~100,000 (approx.)",
        parent="Publicly traded (NYSE: DAL)",
        alliance="SkyTeam",
        iata="DL", icao="DAL",
        notes="One of the world's largest airlines by fleet and passengers."),
    "american-airlines": dict(
        name="American Airlines, Inc.",
        founders="Formed from a consolidation of ~80 carriers (American Airways); C.R. Smith an early leader",
        founded="1930 (American Airways); 1934 as American Airlines",
        hq="Fort Worth, Texas, USA",
        offices="Hubs: Dallas/Fort Worth, Charlotte, Phoenix, Miami, Chicago O'Hare, Philadelphia, Washington DCA, New York (JFK/LGA), Los Angeles",
        website="https://www.aa.com",
        contact="+1-800-433-7300 (US reservations); https://www.aa.com/contact",
        employees="~130,000 (approx., American Airlines Group)",
        parent="American Airlines Group Inc. (NASDAQ: AAL)",
        alliance="oneworld (founding member)",
        iata="AA", icao="AAL",
        notes="Largest airline in the world by scheduled passengers and fleet."),
    "united-airlines": dict(
        name="United Airlines, Inc.",
        founders="Walter Varney (Varney Air Lines, antecedent); William Boeing (United Aircraft and Transport)",
        founded="1926 (Varney Air Lines); United Air Lines 1931",
        hq="Chicago, Illinois, USA (Willis Tower)",
        offices="Hubs: Chicago O'Hare, Denver, Houston (IAH), Newark, San Francisco, Washington Dulles, Los Angeles",
        website="https://www.united.com",
        contact="+1-800-864-8331 (US reservations); https://www.united.com/en/us/customer-care",
        employees="~100,000 (approx.)",
        parent="United Airlines Holdings, Inc. (NASDAQ: UAL)",
        alliance="Star Alliance (founding member)",
        iata="UA", icao="UAL",
        notes="Major US network carrier."),
    "southwest-airlines": dict(
        name="Southwest Airlines Co.",
        founders="Herb Kelleher and Rollin King",
        founded="1967 (as Air Southwest); flights began 1971",
        hq="Dallas, Texas, USA (Love Field)",
        offices="Major bases: Dallas Love, Chicago Midway, Las Vegas, Baltimore/Washington, Denver, Houston Hobby, Phoenix",
        website="https://www.southwest.com",
        contact="+1-800-435-9792 (US); https://www.southwest.com/contact-us/",
        employees="~75,000 (approx.)",
        parent="Publicly traded (NYSE: LUV)",
        alliance="None (independent low-cost carrier)",
        iata="WN", icao="SWA",
        notes="World's largest low-cost carrier by passengers carried."),
    "alaska-airlines": dict(
        name="Alaska Airlines, Inc.",
        founders="Linious 'Mac' McGee (McGee Airways antecedent)",
        founded="1932 (McGee Airways); Alaska Airlines name 1944",
        hq="SeaTac, Washington, USA",
        offices="Hubs: Seattle/Tacoma, Portland, Anchorage, San Francisco, Los Angeles",
        website="https://www.alaskaair.com",
        contact="+1-800-252-7522 (US); https://www.alaskaair.com/content/about-us/contact-us",
        employees="~23,000 (approx., Alaska Air Group)",
        parent="Alaska Air Group, Inc. (NYSE: ALK)",
        alliance="oneworld",
        iata="AS", icao="ASA",
        notes="West-coast focused US carrier; parent of Hawaiian Airlines since 2024."),
    "jetblue-airways": dict(
        name="JetBlue Airways Corporation",
        founders="David Neeleman",
        founded="1998 (incorporated); flights began 2000",
        hq="Long Island City, New York, USA",
        offices="Focus cities: New York JFK, Boston, Fort Lauderdale, Orlando, San Juan",
        website="https://www.jetblue.com",
        contact="+1-800-538-2583 (1-800-JETBLUE); https://www.jetblue.com/contact-us",
        employees="~22,000 (approx.)",
        parent="Publicly traded (NASDAQ: JBLU)",
        alliance="None (interline/partnerships)",
        iata="B6", icao="JBU",
        notes="US low-cost/hybrid carrier."),
    "spirit-airlines": dict(
        name="Spirit Airlines, LLC",
        founders="Ned Homfeld (as Clippert Trucking / Charter One antecedent)",
        founded="1983 (Charter One); Spirit Airlines name 1992",
        hq="Dania Beach, Florida, USA",
        offices="Bases: Fort Lauderdale, Orlando, Las Vegas, Detroit, Dallas/Fort Worth, Atlantic City",
        website="https://www.spirit.com",
        contact="+1-855-728-3555 (US); https://customersupport.spirit.com",
        employees="~13,000 (approx.)",
        parent="Spirit Aviation Holdings (restructured 2025)",
        alliance="None (ultra-low-cost carrier)",
        iata="NK", icao="NKS",
        notes="US ultra-low-cost carrier."),
    "frontier-airlines": dict(
        name="Frontier Airlines, Inc.",
        founders="Re-established as a low-cost carrier (modern Frontier, 1994)",
        founded="1994 (modern Frontier; original Frontier 1950)",
        hq="Denver, Colorado, USA",
        offices="Primary base: Denver; focus cities incl. Las Vegas, Orlando, Phoenix-Mesa",
        website="https://www.flyfrontier.com",
        contact="+1-801-401-9000; https://www.flyfrontier.com/travel/traveler-info/contact-us/",
        employees="~7,000 (approx.)",
        parent="Frontier Group Holdings, Inc. (NASDAQ: ULCC)",
        alliance="None (ultra-low-cost carrier)",
        iata="F9", icao="FFT",
        notes="US ultra-low-cost carrier."),
    "hawaiian-airlines": dict(
        name="Hawaiian Airlines, Inc.",
        founders="Stanley Kennedy (as Inter-Island Airways)",
        founded="1929 (as Inter-Island Airways); Hawaiian Airlines name 1941",
        hq="Honolulu, Hawaii, USA",
        offices="Hubs: Honolulu, Kahului (Maui)",
        website="https://www.hawaiianairlines.com",
        contact="+1-800-367-5320 (US); https://www.hawaiianairlines.com/contact-us",
        employees="~7,000 (approx.)",
        parent="Alaska Air Group (acquired 2024)",
        alliance="oneworld (via Alaska Air Group)",
        iata="HA", icao="HAL",
        notes="Hawaii's largest and longest-serving carrier."),
    "air-canada": dict(
        name="Air Canada",
        founders="Government of Canada (as Trans-Canada Air Lines)",
        founded="1937 (as Trans-Canada Air Lines); renamed Air Canada 1965",
        hq="Saint-Laurent, Montreal, Quebec, Canada",
        offices="Hubs: Toronto Pearson, Montreal-Trudeau, Vancouver; base Calgary",
        website="https://www.aircanada.com",
        contact="+1-888-247-2262 (North America); https://www.aircanada.com/contact",
        employees="~40,000 (approx.)",
        parent="Publicly traded (TSX: AC)",
        alliance="Star Alliance (founding member)",
        iata="AC", icao="ACA",
        notes="Flag carrier and largest airline of Canada."),
    "westjet": dict(
        name="WestJet Airlines Ltd.",
        founders="Clive Beddoe, Mark Hill, Tim Morgan, Donald Bell",
        founded="1996",
        hq="Calgary, Alberta, Canada",
        offices="Hubs: Calgary, Toronto Pearson, Vancouver",
        website="https://www.westjet.com",
        contact="+1-888-937-8538 (1-888-WESTJET); https://www.westjet.com/en-ca/contact-us",
        employees="~14,000 (approx.)",
        parent="Owned by Onex Corporation (private, since 2019)",
        alliance="None (partnerships; Delta/Air France-KLM investment)",
        iata="WS", icao="WJA",
        notes="Canada's second-largest airline."),
    "aeromexico": dict(
        name="Aerovias de Mexico, S.A. de C.V. (Aeromexico)",
        founders="Antonio Diaz Lombardo (as Aeronaves de Mexico)",
        founded="1934 (as Aeronaves de Mexico)",
        hq="Mexico City, Mexico",
        offices="Hubs: Mexico City (AICM/AIFA), Monterrey, Guadalajara",
        website="https://www.aeromexico.com",
        contact="+52-55-5133-4000; https://www.aeromexico.com/en-us/help-center",
        employees="~14,000 (approx.)",
        parent="Grupo Aeromexico",
        alliance="SkyTeam",
        iata="AM", icao="AMX",
        notes="Flag carrier of Mexico."),
    "british-airways": dict(
        name="British Airways Plc",
        founders="Formed by merger of BOAC and BEA (state-created)",
        founded="1974 (merger of BOAC & BEA; roots to 1919 AT&T)",
        hq="Harmondsworth, London (Waterside), United Kingdom",
        offices="Hubs: London Heathrow, London Gatwick, London City",
        website="https://www.britishairways.com",
        contact="+44-344-493-0787 (UK); https://www.britishairways.com/en-gb/information/help-and-contacts",
        employees="~35,000 (approx.)",
        parent="International Airlines Group (IAG)",
        alliance="oneworld (founding member)",
        iata="BA", icao="BAW",
        notes="Flag carrier of the United Kingdom."),
    "lufthansa": dict(
        name="Deutsche Lufthansa AG",
        founders="Re-established 1953 (original Deutsche Luft Hansa 1926)",
        founded="1953 (modern Lufthansa; original 1926)",
        hq="Cologne, Germany (operational base Frankfurt)",
        offices="Hubs: Frankfurt, Munich",
        website="https://www.lufthansa.com",
        contact="+49-69-86799799; https://www.lufthansa.com/us/en/help-and-contact",
        employees="~96,000 (approx., Lufthansa Group)",
        parent="Lufthansa Group (FRA: LHA)",
        alliance="Star Alliance (founding member)",
        iata="LH", icao="DLH",
        notes="Flag carrier of Germany and largest German airline."),
    "air-france": dict(
        name="Societe Air France, S.A.",
        founders="Formed by merger of several carriers (incl. Air Orient, Air Union)",
        founded="1933",
        hq="Tremblay-en-France, Paris, France",
        offices="Hubs: Paris Charles de Gaulle, Paris Orly",
        website="https://www.airfrance.com",
        contact="+33-9-69-39-36-54; https://www.airfrance.us/information/contacts",
        employees="~41,000 (approx.)",
        parent="Air France-KLM Group",
        alliance="SkyTeam (founding member)",
        iata="AF", icao="AFR",
        notes="Flag carrier of France."),
    "klm": dict(
        name="Koninklijke Luchtvaart Maatschappij N.V. (KLM Royal Dutch Airlines)",
        founders="Albert Plesman",
        founded="1919",
        hq="Amstelveen, Netherlands",
        offices="Hub: Amsterdam Schiphol",
        website="https://www.klm.com",
        contact="+31-20-474-7747; https://www.klm.com/information/contact",
        employees="~33,000 (approx.)",
        parent="Air France-KLM Group",
        alliance="SkyTeam (founding member)",
        iata="KL", icao="KLM",
        notes="Oldest airline still operating under its original name (est. 1919)."),
    "ryanair": dict(
        name="Ryanair DAC",
        founders="Christopher Ryan, Liam Lonergan, Tony Ryan",
        founded="1984",
        hq="Swords, Dublin, Ireland (Airside Business Park)",
        offices="Major bases: Dublin, London Stansted, Milan Bergamo, Brussels Charleroi",
        website="https://www.ryanair.com",
        contact="https://www.ryanair.com/gb/en/useful-info/help-centre (web chat)",
        employees="~27,000 (approx., Ryanair Group)",
        parent="Ryanair Holdings plc (ISE/LSE: RYA / RYAAY)",
        alliance="None (ultra-low-cost carrier)",
        iata="FR", icao="RYR",
        notes="Europe's largest airline by passengers carried."),
    "easyjet": dict(
        name="easyJet plc",
        founders="Stelios Haji-Ioannou",
        founded="1995",
        hq="Luton, Bedfordshire, United Kingdom",
        offices="Major bases: London Gatwick, London Luton, Milan Malpensa, Geneva, Berlin",
        website="https://www.easyjet.com",
        contact="https://www.easyjet.com/en/help/contact (web/chat)",
        employees="~14,000 (approx.)",
        parent="easyJet plc (LSE: EZJ)",
        alliance="None (low-cost carrier)",
        iata="U2", icao="EZY",
        notes="Major European low-cost carrier."),
    "iberia": dict(
        name="Iberia Lineas Aereas de Espana, S.A.",
        founders="Horacio Echeberrieta (founder of the original company)",
        founded="1927",
        hq="Madrid, Spain",
        offices="Hub: Madrid-Barajas (Adolfo Suarez)",
        website="https://www.iberia.com",
        contact="+34-901-111-500; https://www.iberia.com/us/customer-service/",
        employees="~16,000 (approx.)",
        parent="International Airlines Group (IAG)",
        alliance="oneworld (founding member)",
        iata="IB", icao="IBE",
        notes="Flag carrier of Spain."),
    "alitalia-ita-airways": dict(
        name="Italia Trasporto Aereo S.p.A. (ITA Airways)",
        founders="Italian State (Ministry of Economy and Finance)",
        founded="2020 (founded); operations began 15 October 2021, succeeding Alitalia",
        hq="Rome, Italy",
        offices="Hubs: Rome Fiumicino; focus Milan Linate",
        website="https://www.itaspa.com",
        contact="+39-06-85960020; https://www.itaspa.com/en_us/support.html",
        employees="~5,000 (approx.)",
        parent="Lufthansa Group (majority) and Italian MEF",
        alliance="SkyTeam",
        iata="AZ", icao="ITY",
        notes="Flag carrier of Italy; successor to Alitalia."),
    "swiss": dict(
        name="Swiss International Air Lines AG",
        founders="Formed from the assets of the defunct Swissair (and Crossair)",
        founded="2002",
        hq="Basel, Switzerland (operational base Zurich)",
        offices="Hubs: Zurich, Geneva",
        website="https://www.swiss.com",
        contact="+41-848-700-700; https://www.swiss.com/us/en/help-and-contact",
        employees="~9,500 (approx.)",
        parent="Lufthansa Group",
        alliance="Star Alliance",
        iata="LX", icao="SWR",
        notes="Flag carrier of Switzerland."),
    "sas": dict(
        name="Scandinavian Airlines System (SAS AB)",
        founders="Consortium of Danish, Norwegian and Swedish carriers",
        founded="1946",
        hq="Solna, Stockholm, Sweden",
        offices="Hubs: Copenhagen, Stockholm Arlanda, Oslo",
        website="https://www.flysas.com",
        contact="https://www.flysas.com/en/customer-service/ (web/chat)",
        employees="~9,000 (approx.)",
        parent="SAS AB (acquired by Air France-KLM/consortium, 2024)",
        alliance="SkyTeam (joined 2024; formerly Star Alliance)",
        iata="SK", icao="SAS",
        notes="Flag carrier of Denmark, Norway and Sweden."),
    "turkish-airlines": dict(
        name="Turk Hava Yollari A.O. (Turkish Airlines)",
        founders="Government of Turkey (as State Airlines Administration)",
        founded="1933",
        hq="Istanbul, Turkey (Istanbul Airport)",
        offices="Hub: Istanbul Airport (IST); secondary Istanbul Sabiha Gokcen, Ankara",
        website="https://www.turkishairlines.com",
        contact="+90-850-333-0849; https://www.turkishairlines.com/en-us/any-questions/",
        employees="~85,000 (approx., group)",
        parent="Turkish Airlines (partly state-owned; BIST: THYAO)",
        alliance="Star Alliance",
        iata="TK", icao="THY",
        notes="Flag carrier of Turkey; flies to the most countries of any airline."),
    "aer-lingus": dict(
        name="Aer Lingus Limited",
        founders="Irish Government (Sean O hUadhaigh among founders)",
        founded="1936",
        hq="Dublin, Ireland (Dublin Airport)",
        offices="Hub: Dublin; bases Cork, Shannon, Manchester",
        website="https://www.aerlingus.com",
        contact="https://www.aerlingus.com/support/contact-us/ (web)",
        employees="~4,000 (approx.)",
        parent="International Airlines Group (IAG)",
        alliance="oneworld (connecting partner)",
        iata="EI", icao="EIN",
        notes="Flag carrier of Ireland."),
    "emirates": dict(
        name="Emirates (The Emirates Group)",
        founders="Government of Dubai; Maurice Flanagan and Sheikh Ahmed bin Saeed Al Maktoum",
        founded="1985",
        hq="Garhoud, Dubai, United Arab Emirates",
        offices="Hub: Dubai International (DXB)",
        website="https://www.emirates.com",
        contact="+971-600-555-555; https://www.emirates.com/us/english/help/",
        employees="~100,000+ (approx., Emirates Group)",
        parent="Investment Corporation of Dubai (government-owned)",
        alliance="None (extensive partnerships)",
        iata="EK", icao="UAE",
        notes="Largest airline in the Middle East; all-widebody fleet."),
    "qatar-airways": dict(
        name="Qatar Airways Q.C.S.C.",
        founders="Government of Qatar (relaunched 1997 under Akbar Al Baker)",
        founded="1993 (operations 1994); relaunched 1997",
        hq="Doha, Qatar (Qatar Airways Tower)",
        offices="Hub: Hamad International Airport, Doha",
        website="https://www.qatarairways.com",
        contact="+974-4023-0000; https://www.qatarairways.com/en/help.html",
        employees="~50,000 (approx., group)",
        parent="Qatar Airways Group (state-owned)",
        alliance="oneworld",
        iata="QR", icao="QTR",
        notes="Flag carrier of Qatar."),
    "etihad-airways": dict(
        name="Etihad Airways P.J.S.C.",
        founders="Government of Abu Dhabi (Royal (Amiri) Decree)",
        founded="2003",
        hq="Abu Dhabi, United Arab Emirates (Khalifa City)",
        offices="Hub: Zayed International Airport, Abu Dhabi",
        website="https://www.etihad.com",
        contact="+971-600-555-666; https://www.etihad.com/en-us/help",
        employees="~10,000+ (approx.)",
        parent="ADQ (Abu Dhabi state-owned)",
        alliance="None (partnerships)",
        iata="EY", icao="ETD",
        notes="Flag carrier of the United Arab Emirates (Abu Dhabi)."),
    "singapore-airlines": dict(
        name="Singapore Airlines Limited",
        founders="Split from Malaysia-Singapore Airlines (roots to Malayan Airways, 1947)",
        founded="1947 (as Malayan Airways); Singapore Airlines 1972",
        hq="Airline House, Singapore (Changi)",
        offices="Hub: Singapore Changi Airport",
        website="https://www.singaporeair.com",
        contact="+65-6223-8888; https://www.singaporeair.com/en_UK/us/contact-us/",
        employees="~15,000 (approx.)",
        parent="Singapore Airlines Group (Temasek majority; SGX: C6L)",
        alliance="Star Alliance",
        iata="SQ", icao="SIA",
        notes="Flag carrier of Singapore."),
    "cathay-pacific": dict(
        name="Cathay Pacific Airways Limited",
        founders="Roy Farrell and Sydney de Kantzow",
        founded="1946",
        hq="Hong Kong International Airport, Hong Kong",
        offices="Hub: Hong Kong International Airport",
        website="https://www.cathaypacific.com",
        contact="+852-2747-3333; https://www.cathaypacific.com/cx/en_US/contact-us.html",
        employees="~26,000 (approx., group)",
        parent="Swire Pacific (major shareholder); HKEX: 0293",
        alliance="oneworld (founding member)",
        iata="CX", icao="CPA",
        notes="Flag carrier of Hong Kong."),
    "qantas": dict(
        name="Qantas Airways Limited",
        founders="Hudson Fysh, Paul McGinness, Fergus McMaster",
        founded="1920 (Queensland and Northern Territory Aerial Services)",
        hq="Mascot, Sydney, New South Wales, Australia",
        offices="Hubs: Sydney, Melbourne, Brisbane, Perth",
        website="https://www.qantas.com",
        contact="+61-13-13-13 (Australia); https://www.qantas.com/us/en/support.html",
        employees="~24,000 (approx.)",
        parent="Qantas Airways Limited (ASX: QAN)",
        alliance="oneworld (founding member)",
        iata="QF", icao="QFA",
        notes="Flag carrier of Australia; one of the world's oldest airlines."),
    "air-new-zealand": dict(
        name="Air New Zealand Limited",
        founders="Union Airways / government (as Tasman Empire Airways, TEAL)",
        founded="1940 (as TEAL); Air New Zealand name 1965",
        hq="Auckland, New Zealand",
        offices="Hub: Auckland; bases Wellington, Christchurch",
        website="https://www.airnewzealand.com",
        contact="+64-800-737-000 (NZ); https://www.airnewzealand.com/contact-us",
        employees="~11,000 (approx.)",
        parent="Air New Zealand Limited (majority NZ govt; NZX: AIR)",
        alliance="Star Alliance",
        iata="NZ", icao="ANZ",
        notes="Flag carrier of New Zealand."),
    "japan-airlines": dict(
        name="Japan Airlines Co., Ltd. (JAL)",
        founders="Government of Japan (established as a semi-governmental carrier)",
        founded="1951",
        hq="Shinagawa, Tokyo, Japan",
        offices="Hubs: Tokyo Haneda, Tokyo Narita; Osaka Kansai/Itami",
        website="https://www.jal.com",
        contact="+81-570-025-031 (Japan); https://www.jal.co.jp/en/inter/contact/",
        employees="~36,000 (approx., group)",
        parent="Japan Airlines Co., Ltd. (TYO: 9201)",
        alliance="oneworld",
        iata="JL", icao="JAL",
        notes="One of two major flag carriers of Japan."),
    "all-nippon-airways": dict(
        name="All Nippon Airways Co., Ltd. (ANA)",
        founders="Formed by merger of Japan Helicopter and Far East Airlines",
        founded="1952 (as Japan Helicopter and Aeroplane Transports)",
        hq="Minato, Tokyo, Japan",
        offices="Hubs: Tokyo Haneda, Tokyo Narita",
        website="https://www.ana.co.jp",
        contact="+81-570-029-333 (Japan); https://www.ana.co.jp/en/us/customer-support/",
        employees="~45,000 (approx., ANA Holdings)",
        parent="ANA Holdings Inc. (TYO: 9202)",
        alliance="Star Alliance",
        iata="NH", icao="ANA",
        notes="Largest airline in Japan by fleet size and passengers."),
    "korean-air": dict(
        name="Korean Air Lines Co., Ltd.",
        founders="Government (as Korean National Airlines); privatized to Hanjin (Cho Choong-hoon)",
        founded="1962 (Korean Air Lines; roots to KNA 1946)",
        hq="Gangseo-gu, Seoul, South Korea",
        offices="Hub: Seoul Incheon; Gimpo",
        website="https://www.koreanair.com",
        contact="+82-1588-2001 (Korea); https://www.koreanair.com/contact-us",
        employees="~20,000 (approx.)",
        parent="Hanjin KAL / Hanjin Group (KRX: 003490)",
        alliance="SkyTeam (founding member)",
        iata="KE", icao="KAL",
        notes="Largest airline and flag carrier of South Korea; acquired Asiana (2024)."),
    "china-southern-airlines": dict(
        name="China Southern Airlines Company Limited",
        founders="Government of China (from CAAC reorganization)",
        founded="1988 (from CAAC Guangzhou operations)",
        hq="Baiyun District, Guangzhou, Guangdong, China",
        offices="Hubs: Guangzhou Baiyun, Beijing Daxing",
        website="https://www.csair.com",
        contact="+86-95539 (China); https://www.csair.com/en/tourguide/contact_us/",
        employees="~100,000 (approx.)",
        parent="China Southern Air Holding (state-owned; SSE: 600029)",
        alliance="None (left SkyTeam 2019; partnerships)",
        iata="CZ", icao="CSN",
        notes="One of China's largest airlines by fleet and passengers."),
    "air-india": dict(
        name="Air India Limited",
        founders="J.R.D. Tata (as Tata Airlines)",
        founded="1932 (as Tata Airlines); Air India name 1946",
        hq="New Delhi, India (Gurugram corporate office)",
        offices="Hubs: Delhi, Mumbai; bases Bengaluru, Kochi",
        website="https://www.airindia.com",
        contact="+91-124-264-1407; https://www.airindia.com/in/en/customer-support.html",
        employees="~18,000+ (approx., group with AIX)",
        parent="Tata Group (Talace Pvt Ltd), since 2022",
        alliance="Star Alliance",
        iata="AI", icao="AIC",
        notes="Flag carrier of India; returned to Tata Group ownership in 2022."),
    "indigo": dict(
        name="InterGlobe Aviation Limited (IndiGo)",
        founders="Rahul Bhatia and Rakesh Gangwal",
        founded="2006",
        hq="Gurugram, Haryana, India",
        offices="Hubs: Delhi, Mumbai, Bengaluru, Hyderabad, Kolkata",
        website="https://www.goindigo.in",
        contact="+91-124-617-3838; https://www.goindigo.in/contact-us.html",
        employees="~37,000 (approx.)",
        parent="InterGlobe Aviation Ltd (NSE: INDIGO)",
        alliance="None (low-cost carrier; partnerships)",
        iata="6E", icao="IGO",
        notes="India's largest airline by passengers and fleet."),
    "thai-airways": dict(
        name="Thai Airways International Public Company Limited",
        founders="Thai Airways Company and SAS (joint venture)",
        founded="1960",
        hq="Bangkok, Thailand (Vibhavadi Rangsit Road)",
        offices="Hub: Bangkok Suvarnabhumi",
        website="https://www.thaiairways.com",
        contact="+66-2-356-1111; https://www.thaiairways.com/en/contact_us/contact.page",
        employees="~15,000 (approx.)",
        parent="Thai Airways International PCL (SET: THAI)",
        alliance="Star Alliance",
        iata="TG", icao="THA",
        notes="Flag carrier of Thailand."),
    "latam-airlines": dict(
        name="LATAM Airlines Group S.A.",
        founders="Merger of LAN Airlines (Chile) and TAM Airlines (Brazil)",
        founded="2012 (merger; LAN roots to 1929, TAM to 1961)",
        hq="Santiago, Chile",
        offices="Hubs: Santiago, Sao Paulo-Guarulhos, Lima, Bogota",
        website="https://www.latamairlines.com",
        contact="https://www.latamairlines.com/us/en/contact-us (web)",
        employees="~30,000+ (approx., group)",
        parent="LATAM Airlines Group S.A. (NYSE: LTM)",
        alliance="None (left oneworld 2020; Delta partnership)",
        iata="LA", icao="LAN",
        notes="Largest airline group in Latin America."),
    "avianca": dict(
        name="Avianca (Aerovias del Continente Americano S.A.)",
        founders="Roots to SCADTA, founded by German and Colombian investors",
        founded="1919 (as SCADTA; Avianca name 1940)",
        hq="Bogota, Colombia",
        offices="Hub: Bogota El Dorado; San Salvador",
        website="https://www.avianca.com",
        contact="+57-601-401-3434; https://www.avianca.com/us/en/customer-service/",
        employees="~14,000 (approx.)",
        parent="Avianca Group International Limited",
        alliance="Star Alliance",
        iata="AV", icao="AVA",
        notes="Flag carrier of Colombia; one of the oldest airlines in the world."),
    "copa-airlines": dict(
        name="Compania Panamena de Aviacion, S.A. (Copa Airlines)",
        founders="Panamanian investors (with early Pan Am involvement)",
        founded="1947",
        hq="Panama City, Panama",
        offices="Hub: Tocumen International Airport, Panama City",
        website="https://www.copaair.com",
        contact="+507-217-2672; https://www.copaair.com/en/web/us/contact-us",
        employees="~9,000 (approx.)",
        parent="Copa Holdings, S.A. (NYSE: CPA)",
        alliance="Star Alliance",
        iata="CM", icao="CMP",
        notes="Flag carrier of Panama; 'Hub of the Americas' at Tocumen."),
    "ethiopian-airlines": dict(
        name="Ethiopian Airlines Group",
        founders="Government of Ethiopia (with TWA assistance)",
        founded="1945 (operations began 1946)",
        hq="Addis Ababa, Ethiopia (Bole International Airport)",
        offices="Hub: Addis Ababa Bole International Airport",
        website="https://www.ethiopianairlines.com",
        contact="+251-116-179-900; https://www.ethiopianairlines.com/aa/contact",
        employees="~17,000+ (approx., group)",
        parent="Ethiopian Airlines Group (state-owned)",
        alliance="Star Alliance",
        iata="ET", icao="ETH",
        notes="Flag carrier of Ethiopia; largest airline in Africa."),
    "south-african-airways": dict(
        name="South African Airways SOC Ltd (SAA)",
        founders="Government of South Africa (from Union Airways)",
        founded="1934",
        hq="Kempton Park, Johannesburg, South Africa",
        offices="Hub: O.R. Tambo International Airport, Johannesburg",
        website="https://www.flysaa.com",
        contact="+27-11-978-1111; https://www.flysaa.com/manage-fly/contact-us",
        employees="~1,000+ (approx., post-restructuring)",
        parent="South African Government / Takatso (restructured)",
        alliance="Star Alliance",
        iata="SA", icao="SAA",
        notes="Flag carrier of South Africa."),
    "kenya-airways": dict(
        name="Kenya Airways PLC",
        founders="Government of Kenya",
        founded="1977",
        hq="Nairobi, Kenya (Embakasi; Jomo Kenyatta Int'l)",
        offices="Hub: Jomo Kenyatta International Airport, Nairobi",
        website="https://www.kenya-airways.com",
        contact="+254-711-024-747; https://www.kenya-airways.com/en/support/contact-us/",
        employees="~4,000 (approx.)",
        parent="Kenya Airways PLC (NSE: KQ)",
        alliance="SkyTeam",
        iata="KQ", icao="KQA",
        notes="Flag carrier of Kenya; 'The Pride of Africa'."),
}

AVG_AGE_NOTE = (
    "Not publicly disclosed by the company. As industry context, the commercial "
    "aviation workforce (across pilots, cabin crew, engineering, and corporate "
    "staff) typically skews to roughly the late-30s to mid-40s on average; treat "
    "this as an estimate, not an official figure."
)


def repo_root() -> Path:
    return Path(__file__).resolve().parent.parent.parent


def render(slug: str, r: dict) -> str:
    return f"""# Establishment — {r['name']}

> Company establishment reference for the `{slug}` brand site in
> `lib/website/{slug}/`. Founding facts (founders, years, headquarters, hubs,
> alliance, parent) are well-established public information. Workforce size is
> approximate and changes over time; the average employee age is an estimate
> (see the note below), as airlines do not generally publish it.

## Company

| Field | Detail |
|---|---|
| **Company name** | {r['name']} |
| **IATA / ICAO code** | {r['iata']} / {r['icao']} |
| **Founder(s)** | {r['founders']} |
| **Founding year(s)** | {r['founded']} |
| **Headquarters** | {r['hq']} |
| **Offices / hubs** | {r['offices']} |
| **Parent / ownership** | {r['parent']} |
| **Alliance** | {r['alliance']} |

## Contact information

- **Website:** {r['website']}
- **General contact:** {r['contact']}

> Phone numbers and support URLs change; always confirm against the official
> website above before relying on them.

## Workforce

- **Employees:** {r['employees']}
- **Average age of employees:** {AVG_AGE_NOTE}

## Notes

{r['notes']}

---

*Maintained alongside the SLeeLa Website Generator brand site in this folder
(see `{slug.split('/')[-1]}`'s `*Website.sleela`). Summary reference only — not a
legal, investor-grade, or real-time record. — MEARVK LLC — 2026*
"""


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--count", action="store_true", help="list targets and exit")
    args = ap.parse_args()
    root = repo_root()
    web = root / "lib" / "website"

    # Every airline folder on disk must have a record, and vice versa.
    folders = sorted(p.name for p in web.iterdir()
                     if p.is_dir())
    missing = [f for f in folders if f not in AIRLINES]
    extra = [s for s in AIRLINES if s not in folders]
    if missing:
        print(f"error: folders without an establishment record: {missing}",
              file=sys.stderr)
        return 2
    if extra:
        print(f"error: records without a folder on disk: {extra}", file=sys.stderr)
        return 2

    count = 0
    for slug, r in AIRLINES.items():
        out = web / slug / "ESTABLISHMENT.md"
        if not args.count:
            out.write_text(render(slug, r), encoding="utf-8")
        count += 1
    print(f"ESTABLISHMENT.md written for {count} airline folders")
    if args.count:
        for s in sorted(AIRLINES):
            print(f"  lib/website/{s}/ESTABLISHMENT.md")
    return 0


if __name__ == "__main__":
    sys.exit(main())
