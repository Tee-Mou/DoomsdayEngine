import type { SetStateAction } from 'react';
import "./Console.css"

interface ConsoleProps {
    consoleOutput: string;
    setConsoleOutput: React.Dispatch<SetStateAction<string>>;
}

function Console ( props: ConsoleProps ) {
    return (
        <div className='console'>
            <div className='content'>
                {props.consoleOutput}
            </div>
        </div>
    )
}

export default Console