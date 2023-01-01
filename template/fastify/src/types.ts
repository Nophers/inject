import { FastifyInstance, FastifyPluginOptions } from "fastify";

/**
 * Small middleware for the routes
 * @param server
 * @returns {Promise<void>}
 */
export interface RoutesOptions extends FastifyPluginOptions {
  server: FastifyInstance;
};