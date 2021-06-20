#include "RVO.h"
#include "Math.h"
#include <map>
#include <iostream>
#include "TextureLoader.h"

Texture RVO::getTexture() { return m_texture; }

RVO::RVO(int agentCount) {
    m_agents = std::vector<RvoAgent>();
    float radiusRvo = 7.5f;

    for (int i = 0; i < agentCount; ++i) {
        float angle = (i / (float)agentCount) * 2 * M_PI;
        float xCoord = cos(angle) * radiusRvo + 8;
        float yCoord = sin(angle) * radiusRvo + 8;
        Vec2 position = Vec2(xCoord, yCoord);
        xCoord = cos(angle + M_PI) * radiusRvo + 8;
        yCoord = sin(angle + M_PI) * radiusRvo + 8;
        Vec2 goal = Vec2(xCoord, yCoord);
        m_agents.emplace_back(position, Vec2(0), goal);
    }

    m_texture = TextureLoader::LoadTexture("blackRvo.png", 1, 0.1, 0.1);
    glBindImageTexture(1, m_texture.GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    glGenBuffers(1, &m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_agents.size() * 6 * sizeof(GLfloat), &m_agents[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  
    m_computeShader = ComputeShader("..\\Res\\Shaders\\ComputeShaders\\rvoShader.txt");
    GLint numAgents = glGetUniformLocation(m_computeShader.m_programID, "numAgents");
    GLint robotRadius = glGetUniformLocation(m_computeShader.m_programID, "robotRadius");
    
    glUseProgram(m_computeShader.m_programID);
    glUniform1i(numAgents, agentCount);
    glUniform1f(robotRadius, 0.3);
}

void RVO::run() {
    glUseProgram(m_computeShader.m_programID);
    glDispatchCompute(10, 1, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
float RVO::distance(std::vector<float>& pos1, std::vector<float>& pos2) {
    return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2)) + 0.001f;
}

std::vector<std::vector<float>> RVO::RVO_update(std::vector<std::vector<float>> x, std::vector<std::vector<float>> v_des, std::vector<std::vector<float>> v_current, RVOModel ws_model) {
    float rob_rad = ws_model.robotRadius + 0.1;
    std::vector<std::vector<float>> v_opt = v_current;
    for (int i = 0; i < x.size(); ++i) {
        std::vector<float> vA = std::vector<float>({ v_current[i][0], v_current[i][1] });
        std::vector<float> pA = std::vector<float>({ x[i][0], x[i][1] });
        std::vector<std::vector<std::vector<float>>> rvo_ba_all = std::vector<std::vector<std::vector<float>>>();
        for (int j = 0; j < x.size(); ++j) {
            if (i != j) {
                std::vector<float> vB = std::vector<float>({ v_current[j][0], v_current[j][1] });
                std::vector<float> pB = std::vector<float>({ x[j][0], x[j][1] });
                std::vector<float> transl_vB_vA = std::vector<float>({ pA[0] + 0.5f * (vB[0] + vA[0]), pA[1] + 0.5f * (vB[1] + vA[1]) });
                float dist_BA = distance(pA, pB);
                float theta_BA = atan2(pB[1] - pA[1], pB[0] - pA[0]);
                if (2 * rob_rad > dist_BA) {
                    dist_BA = 2 * rob_rad;
                }
                float theta_BAort = asin(2 * rob_rad / dist_BA);
                float theta_ort_left = theta_BA + theta_BAort;
                std::vector<float> bound_left = std::vector<float>({ cos(theta_ort_left), sin(theta_ort_left) });
                float theta_ort_right = theta_BA - theta_BAort;
                std::vector<float> bound_right = std::vector<float>({ cos(theta_ort_right), sin(theta_ort_right) });

                std::vector<std::vector<float>> rvo_ba = std::vector<std::vector<float>>();
                rvo_ba.push_back(transl_vB_vA);
                rvo_ba.push_back(bound_left);
                rvo_ba.push_back(bound_right);
                rvo_ba.push_back({ dist_BA, 2 * rob_rad });
                rvo_ba_all.push_back(rvo_ba);
            }
        }
        for (std::vector<float> hole : ws_model.circularObstacles) {
            std::vector<float> vB = std::vector<float>({ 0, 0 });
            std::vector<float> pB = std::vector<float>({ hole[0], hole[1] });

            std::vector<float> transl_vB_vA = std::vector<float>({ pA[0] + vB[0], pA[1] + vB[1] });
            float dist_BA = distance(pA, pB);
            float theta_BA = atan2(pB[1] - pA[1], pB[0] - pA[0]);
            float OVER_APPROX_C2S = 1.5;
            float rad = hole[2] * OVER_APPROX_C2S;
            if ((rad + rob_rad) > dist_BA) {
                dist_BA = rad + rob_rad;
            }

            float theta_BAort = asin((rad + rob_rad) / dist_BA);
            float theta_ort_left = theta_BA + theta_BAort;
            std::vector<float> bound_left = std::vector<float>({ cos(theta_ort_left), sin(theta_ort_left) });
            float theta_ort_right = theta_BA - theta_BAort;
            std::vector<float> bound_right = std::vector<float>({ cos(theta_ort_right), sin(theta_ort_right) });

            std::vector<std::vector<float>> rvo_ba = std::vector<std::vector<float>>();
            rvo_ba.push_back(transl_vB_vA);
            rvo_ba.push_back(bound_left);
            rvo_ba.push_back(bound_right);
            rvo_ba.push_back({ dist_BA, rad + rob_rad });
            rvo_ba_all.push_back(rvo_ba);
        }

        std::vector<float> vA_post = intersect(pA, v_des[i], rvo_ba_all);
        v_opt[i] = vA_post;
    }

    return v_opt;
}

std::vector<float> RVO::intersect(std::vector<float> pA, std::vector<float> vA, std::vector<std::vector<std::vector<float>>> rvo_ba_all) {
    float norm_v = distance(vA, std::vector<float>({ 0, 0 }));
    std::vector<std::vector<float>> suitable_V = std::vector<std::vector
        <float>>();
    std::vector<std::vector<float>> unsuitable_V = std::vector<std::vector<float>>();
    for (float theta = 0.0f; theta < 2 * M_PI; theta += 0.1f) {
        for (float rad = 0.02f; rad < norm_v + 0.02f; rad += norm_v / 5.0f) {
            std::vector<float> new_v = std::vector<float>({ rad * cos(theta), rad * sin(theta)});
            bool suit = true;
            for(int i = 0; i < rvo_ba_all.size(); ++i) { 
                float dif0 = new_v[0] + pA[0] - rvo_ba_all[i][0][0];
                float dif1 = new_v[1] + pA[1] - rvo_ba_all[i][0][1];
                
                float theta_dif = atan2(dif1, dif0);
                float theta_right = atan2(rvo_ba_all[i][2][1], rvo_ba_all[i][2][0]);
                float theta_left = atan2(rvo_ba_all[i][1][1], rvo_ba_all[i][1][0]);
                if(in_between(theta_right, theta_dif, theta_left)) {
                    suit = false;
                    break;
                }
            }

            if (suit) {
                suitable_V.push_back(new_v);
            } else {
                unsuitable_V.push_back(new_v);
            }
        }
    }

    std::vector<float> new_v = vA;
    bool suit = true;
    for(int i = 0; i < rvo_ba_all.size(); ++i) {
        float dif0 = new_v[0] + pA[0] - rvo_ba_all[i][0][0];
        float dif1 = new_v[1] + pA[1] - rvo_ba_all[i][0][1];

        float theta_dif = atan2(dif1, dif0);
        float theta_right = atan2(rvo_ba_all[i][2][1], rvo_ba_all[i][2][0]);
        float theta_left = atan2(rvo_ba_all[i][1][1], rvo_ba_all[i][1][0]);
        if(in_between(theta_right, theta_dif, theta_left)) {
            suit = false;
            break;
        }
    }

    if (suit) {
        suitable_V.push_back(new_v);
    }
    else {
        unsuitable_V.push_back(new_v);
    }

    std::vector<float> vA_post = std::vector<float>();
    if (suitable_V.size() > 0) {
        float minimum = 9999999.0f;
        int minimumIndex = -1;
        for (int i = 0; i < suitable_V.size(); ++i) {
            float newDist = distance(vA, suitable_V[i]);
            if (newDist < minimum) {
                minimum = newDist;
                minimumIndex = i;
            }
        }
        vA_post = suitable_V[minimumIndex];
        new_v = vA_post;
    }
    else {
        std::map<std::pair<float, float>, float> tc_V = std::map<std::pair<float, float>, float>();
        for (std::vector<float> unsuit_v : unsuitable_V) {
            tc_V[{unsuit_v[0], unsuit_v[1]}] = 0;
            std::vector<float> tc = std::vector<float>();
            for(int i = 0; i < rvo_ba_all.size(); ++i) {
                float dist = rvo_ba_all[i][3][0];
                float rad = rvo_ba_all[i][3][1];
                float dif0 = unsuit_v[0] + pA[0] - rvo_ba_all[i][0][0];
                float dif1 = unsuit_v[1] + pA[1] - rvo_ba_all[i][0][1];

                float theta_dif = atan2(dif1, dif0);
                float theta_right = atan2(rvo_ba_all[i][2][1], rvo_ba_all[i][2][0]);
                float theta_left = atan2(rvo_ba_all[i][1][1], rvo_ba_all[i][1][0]);
                if (in_between(theta_right, theta_dif, theta_left)) {
                    float small_theta = abs(theta_dif - 0.5 * (theta_left + theta_right));
                    if (abs(dist * sin(small_theta)) >= rad) {
                        rad = abs(dist * sin(small_theta));
                    }
                    float big_theta = asin(abs(dist * sin(small_theta)) / rad);
                    float dist_tg = abs(dist * cos(small_theta)) - abs(rad * cos(big_theta));
                    if (dist_tg < 0) {
                        dist_tg = 0;
                    }
                    float tc_v = dist_tg / distance(std::vector<float>({ dif0, dif1 }), std::vector<float>({ 0.0f, 0.0f }));
                    tc.push_back(tc_v);
                }
            }

            float minimum = 999999.0f;
            for (int i = 0; i < tc.size(); ++i) {
                if (tc[i] < minimum) {
                    minimum = tc[i];
                }
            }

            tc_V[{unsuit_v[0], unsuit_v[1]}] = minimum + 0.001f;
        }
        float WT = 0.2f;
        float minimum = 999999.0f;
        int minimumIndex = -1;
        for (int i = 0; i < unsuitable_V.size(); ++i) {
            float newMinimum = WT / tc_V[{unsuitable_V[i][0], unsuitable_V[i][1]}] + distance(unsuitable_V[i], vA);
            if (newMinimum < minimum) {
                minimum = newMinimum;
                minimumIndex = i;
            }
        }
        vA_post = unsuitable_V[minimumIndex];
    }

    return vA_post;
}

bool RVO::in_between(float theta_right, float theta_dif, float theta_left) {
    if (abs(theta_right - theta_left) <= M_PI) {
        if (theta_right <= theta_dif && theta_dif <= theta_left) {
            return true;
        }
        else {
            return false;
        }
    } else {
        if ((theta_left < 0) && (theta_right > 0)) {
            theta_left += 2 * M_PI;
            if (theta_dif < 0) {
                theta_dif += 2 * M_PI;
            }
            if (theta_right <= theta_dif && theta_dif <= theta_left) {
                return true;
            } else {
                return false;
            }
        }
        if ((theta_left > 0) && (theta_right < 0)) {
            theta_right += 2 * M_PI;
            if (theta_dif < 0) {
                theta_dif += 2 * M_PI;
            }
            if (theta_right <= theta_dif && theta_dif <= theta_left) {
                return true;
            } else {
                return false;
            }
        }
    }
}

std::vector<std::vector<float>> RVO::compute_V_des(std::vector<std::vector<float>> x, std::vector<std::vector<float>> goal, std::vector<float> v_max) {
    std::vector<std::vector<float>> v_des = std::vector<std::vector<float>>();
    for (int i = 0; i < x.size(); ++i) {
        std::vector<float> dif_x = std::vector<float>({ goal[i][0] - x[i][0], goal[i][1] - x[i][1] });
        std::vector<float> temp = std::vector<float>({ 0.0f, 0.0f });
        float norm = distance(dif_x, temp);
        std::vector<float> norm_dif_x = std::vector<float>({ dif_x[0] * v_max[0] / norm, dif_x[1] * v_max[1] / norm });
        v_des.push_back(norm_dif_x);
        if (reach(x[i], goal[i], 0.1)) {
            v_des[i][0] = 0;
            v_des[i][1] = 0;
        }
    }

    return v_des;
}

bool RVO::reach(std::vector<float> p1, std::vector<float> p2, float bound) {
    if(distance(p1, p2) < bound) {
        return true;
    } else {
        return false;
    }
}