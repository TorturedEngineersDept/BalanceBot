import React, { useState } from 'react';
import './ToggleSwitch.css';

const SwitchOptions = {
  OPTION1: "Manual",
  OPTION2: "Autonomous",
  OPTION3: "Mapping"
};

const ToggleSwitch = () => {
  const [activeOption, setActiveOption] = useState(SwitchOptions.OPTION1);

  const getOptionIndex = (option) => {
    return Object.values(SwitchOptions).indexOf(option);
  };

  return (
    <div className="background">
      <div className="SwitchContainer">
        <div
          className="Slider"
          style={{
            transform: `translateX(${getOptionIndex(activeOption) * 100}%)`
          }}
        />
        <div
          className="ToggleItem"
          onClick={() => setActiveOption(SwitchOptions.OPTION1)}
        >
          <div className="Text">Manual</div>
        </div>
        <div
          className="ToggleItem"
          onClick={() => setActiveOption(SwitchOptions.OPTION2)}
        >
          <div className="Text">Autonomous</div>
        </div>
        <div
          className="ToggleItem"
          onClick={() => setActiveOption(SwitchOptions.OPTION3)}
        >
          <div className="Text">Mapping</div>
        </div>
      </div>
    </div>
  );
};

export default ToggleSwitch;
