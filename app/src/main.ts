import { BrowserWindow } from 'electron'
var path = require("path")

export default class Main {
    static mainWindow: Electron.BrowserWindow | null;
    static application: Electron.App;
    static BrowserWindow;

    private static onWindowAllClosed() {
        if (process.platform !== 'darwin') {
            Main.application.quit()
        }
    }

    private static onClose() {
        Main.mainWindow = null;
    }

    private static onReady() {
        Main.mainWindow = new Main.BrowserWindow({
            width: 800, 
            height: 600,
            titleBarStyle: "hidden",
            ...(process.platform !== 'darwin' ? { titleBarOverlay: true } :{})
        });
        Main.mainWindow!
            .loadURL(__dirname+'/../index.html')
        Main.mainWindow!.on('closed', Main.onClose);
    }

    static main(app: Electron.App, browserWindow: typeof BrowserWindow) {
        Main.BrowserWindow = browserWindow;
        Main.application = app;
        Main.application.on('window-all-closed', Main.onWindowAllClosed);
        Main.application.on('ready', Main.onReady);
    }
}