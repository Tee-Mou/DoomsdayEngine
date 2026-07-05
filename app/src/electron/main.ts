import { app, BrowserWindow, ipcMain } from 'electron';
import path from 'path';
import { isDev } from './util.js';
import { spawn } from 'child_process';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);
import type { ChildProcessWithoutNullStreams } from 'child_process';

let mainWindow: BrowserWindow;
let backend: ChildProcessWithoutNullStreams;

function startBackend() {
    backend = spawn(path.join(__dirname, '../../engine/build/Debug/doomsdayEngine.exe'))

    backend.on('error', (err) => {
        console.error('Failed to start backend:', err);
    });

    backend.stdout.on('data', (data) => {
        const lines = data.toString().trim().split('\n');
        lines.forEach((line: string) => {
            mainWindow.webContents.send('backend-response', line.trim());
        });
    });

    backend.stderr.on('data', (data) => {
        console.error('Backend error:', data.toString());
    });

    backend.on('close', (code) => {
        console.log('Backend exited with code:', code);
    });
}

app.on("ready", () => {
    startBackend();
    mainWindow = new BrowserWindow({
        webPreferences: {
            preload: path.join(__dirname, '/preload.cjs'),
        },
    });
    if (isDev()) {
        mainWindow.loadURL("http://localhost:4860");
    }
    else {
        mainWindow.loadFile(path.join(app.getAppPath(), '/dist-react/index.html'))
    }
});

ipcMain.on('send-command', (event, command) => {
    if (backend && backend.stdin.writable) {
        backend.stdin.write(command + '\n');
    }
});

app.on('window-all-closed', () => {
    if (backend) backend.kill();
    app.quit();
});