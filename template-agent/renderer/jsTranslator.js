    import { ScAddr, ScConstruction } from "ts-sc-client";
    import { client } from "../renderer/sc-client.js";

    const construction = new ScConstruction();

    construction.generateNode(ScType.ConstNode, agent);
    construction.generateNode(ScType.ConstNode, actionNode);

    const action_generate_template = "action_generate_template";
    const action_initiated = "action_initiated";
    const button_id = "{{button_id}}";
    const rrel_1 = "rrel_1";
    const keynodes = [
      { id: action_generate_template, type: ScType.ConstNodeClass },
      { id: action_initiated, type: ScType.ConstNodeClass },
      { id: button_id, type: ScType.ConstNode }
    ];
    const res = await client.resolveKeynodes(keynodes);

    construction.generateConnector(
      ScType.ConstPermPosArc,
      res[action_generate_template],
      actionNode
    );
    construction.generateConnector(
      ScType.ConstPermPosArc,
      res[action_initiated],
      actionNode
    );
    construction.generateConnector(
      ScType.ConstPermPosArc,
      actionNode,
      res[button_id],
      "_arc"
    );
    construction.generateConnector(
      ScType.ConstPermPosArc,
      res[rrel_1],
      "_arc"
    );
    await client.generateElements(construction);