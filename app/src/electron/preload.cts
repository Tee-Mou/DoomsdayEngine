const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
    sendCommand: (command: string) => ipcRenderer.send('send-command', command),
    onResponse:  (callback: Function) => ipcRenderer.on('backend-response', (_: any, data: any) => callback(data)),
});