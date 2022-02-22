from google.oauth2 import service_account
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError
import requests
import time
from threading import Timer


NODEMCU_IP = '192.168.1.145'
GET_ID_ROUTE = '/getCurrentID'
SPREADSHEET_ID = '13WlgxBmsFN52TfJ84lMdOJiY5h_mu4NoVKvLo8De1GQ' # The spreadsheet ID 
SHEET_NAME = 'sheet1'
HEADER_RANGE = 'A1:E1' # FIRST_COLUMN:LAST_COLUMN
SERVICE_ACCOUNT_FILE = './service_key.json'



class NFCReader():
    ids = []
    lastId = ""
    currentId = ""

    def getID(self):
        self.currentId = self.getCurrentId()

        if self.lastId != self.currentId:
            self.addToQueue(self.currentId)
        
        self.lastId = self.currentId
            

    def getCurrentId(self):
        res = requests.get(f'http://{NODEMCU_IP}{GET_ID_ROUTE}')
        return res.text


    def addToQueue(self, id):
        if not id in self.ids and id != '':
            self.ids.append(id)

    
    def resetLastId(self):
        if self.lastId != self.currentId:
            self.lastId = ""
            
        Timer(5, self.resetLastId).start()



class SheetWriter:

    def __init__(self):
        creds = service_account.Credentials.from_service_account_file(SERVICE_ACCOUNT_FILE,
                                                 scopes=['https://www.googleapis.com/auth/spreadsheets'])

        service = build('sheets', 'v4', credentials=creds)

        # Call the Sheets API
        self.sheet = service.spreadsheets()

    
    def append(self, id):
        try:
            res = self.sheet.values().append(
                spreadsheetId=SPREADSHEET_ID,
                range=f"{SHEET_NAME}!{HEADER_RANGE}",
                valueInputOption="USER_ENTERED",
                insertDataOption="INSERT_ROWS",
                body={"values" : [[id]]}
            ).execute()

            # print(res)
        
        except HttpError as err:
            print(err)



def main():
    nfcReader = NFCReader()
    sheetWriter = SheetWriter()
    
    nfcReader.resetLastId()

    while True:
        
        nfcReader.getID()
        
        if len(nfcReader.ids) > 0:
            print(nfcReader.ids)
            sheetWriter.append(nfcReader.ids.pop(0))

        time.sleep(0.5)

    
if __name__ == '__main__':
    main()