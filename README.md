# FixedIncome

It's a application impletments the features listed in FINALfinalGUI_reqs.xls.

Include calculate in VaRs, 2 year hedge, Daily Change by Issuer, Risk by Maturity, etc.

Project F is the server site.
Put datas in opening book, closing book, curve, also daily changes files.
Locate to the Project F folder and use command: ./run.sh to make files and run the server
run.sh makes the makefile.1  output the myserver, then run myserver

Client site & GUI code is written by java called Client.
Locate to the client folder, run the client to connect the server and show the GUI.
Before running it, we need to change the hostname in Socket(“hose_name”,0x1234) in client.java file to make sure they can connected.
Client talk to the server, let it run the my server main functions and return the time info and running result that client requested to client, client show them out in the GUI.
