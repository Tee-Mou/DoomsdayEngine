export interface IElectronAPI {
  sendCommand: (command: string) => void;
  onResponse: (callback: (data: string) => void) => void;
}

declare global {
  interface Window {
    electronAPI: IElectronAPI;
  }
}