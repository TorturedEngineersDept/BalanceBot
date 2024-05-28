import ToggleSwitch from '../components/ToggleSwitch';
import Battery from '../components/Battery';

import './BotSpecs.css';

const BotSpecs = () => {

    return (
        <div className="BotSpecs">
            <ToggleSwitch />
            <div className="space" />
            <div className="battery">
                <Battery />
            </div>
        </div>
    );
};

export default BotSpecs;
